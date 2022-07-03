#include "../../osborn/uavs/UAVMobility.h"

#include <iostream>
#include <queue>

//#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/mission/DependentTask.h"
#include "../communication/uav/UAVMysCommunication.h"
#include "TaskAssistant.h"
#include "consensus/ConsensusAlgorithm.h"
//#include "../../src/communication/ModuleMessage.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

Coord position[NUMUAVS];
double velocidade[NUMUAVS];
float bateria[NUMUAVS];
double tempoVoo[NUMUAVS];
bool ativo[NUMUAVS];
int itera[NUMUAVS];
std::vector<Task> tasksVector[NUMUAVS];
std::vector<Coordinate> splitGoTos[NUMUAVS];
std::vector<ModuleMessage> msgs[NUMUAVS];
UAVMysCommunication uavs[NUMUAVS];
int consensusStage[NUMUAVS];

int waypoints[NUMUAVS];
using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

//Base Method
void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());

    initStoppedUAVs();

    initAuxiliarTasksVariables();

    //testeConsensus();

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade[uav.getID()] = par("speed").operator double();
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
    this->rescueDataAndStoreVariables();
}

//Base Method
void UAVMobility::setTargetPosition() {

    //caso tenha esperado
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    }

    //Para Slave
    //Caso esteja na posição inicial, fica inativo e posicionado
    //Caso já tenha iniciado, fique pairando
    else if((UAVRole == UAVMobility::ROLE_SLAVE || UAVRole == UAVMobility::ROLE_LEADER) && !ativo[uav.getID()]){
        if(uav.getID() == 0){
            cout << "SLAVE PARADÃO " << uav.getID() << " Type: " << UAVRole << endl;
        }
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = true;

        targetPosition = assignCoordinate(lastPosition);
    }

    //Caso seja lider inativo
    else if(UAVRole == UAVMobility::ROLE_DISABLED && !ativo[uav.getID()]) {
        Coord inicialPosition(10, 0, 10);

        if(targetPosition != inicialPosition){
            targetPosition = assignCoordinate(inicialPosition);
            double speed = speedParameter->doubleValue();
            double distance = lastPosition.distance(targetPosition);
            simtime_t travelTime = distance / speed;
            nextChange = simTime() + travelTime;
            nextMoveIsWait = hasWaitTime;
        }else{
            simtime_t waitTime = waitTimeParameter->doubleValue()+3;
            nextChange = simTime() + waitTime;
            nextMoveIsWait = false;
        }
    }

    //Caso contrário
    else {
        cout << "[u"<<uav.getID()<<"]itera: " << itera[uav.getID()] << " QTDTasks: " << tasksVector[uav.getID()].size() << endl;
        //Se tiver tarefas para fazer
        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){ //if there are tasks not performed
            cout << "[u"<<uav.getID()<<"]Status da tarefa atual: " << tasksVector[uav.getID()][itera[uav.getID()]].getStatus() << endl;
            //std::cout << "TEM TAREFA!" << std::endl;
            int task = itera[uav.getID()];
            //finalizando
            if(tasksVector[uav.getID()][task].getStatus() == Task::COMPLETED){
                tasksVector[uav.getID()][task].setComplete();

                //enviando mensagem de finalizada
                NetworkConfigurations ntc;
                ntc.setIdSocket(uavs[uav.getID()].getSocketCode());
                this->uav.setNetworkConfigurations(ntc);
                UAVMysCommunication u;
                u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
                u.setSelfID(this->uav.getID());
                TaskMessage msg;
                msg.setCode(273);
                msg.setSource(u.getSelfID());
                msg.setTask(tasksVector[uav.getID()][task]);
                u.dispatchTaskMessage(msg);

                //next task
                itera[uav.getID()]++;
                cout << "UAV"<<uav.getID()<< " COMPLETOU TAREFA, PARTINDO PARA A PRÓXIMA!";

            }else{
                cout << "para executar tarefa > ";
                //TaskMessage tm("checking", 123);

                cout << "[zzz] Mensagens de: " << uav.getID() << endl;

                if(consensusStage[uav.getID()] < 2){
                    ModuleMessage mm(strdup("location"), 234, 1);

                    mm.setSource(uav.getID());
                    mm.setDestination(-2); //Numero qualquer, pois vai para todos
                    sendMessageToModule(mm);

                    consensusStage[uav.getID()] = 1; //solicitou coordenadas
                }

                std::vector<UAV> listaUAVs;
                verificarMensagens(&listaUAVs);


                if(consensusStage[uav.getID()] == 2){
                    Coordinate coordUAV = castCoordToCoordinate(position[this->uav.getID()]);
                    cout << "[CONSENSUS]: NUMERO DE UAVS PARA O CONSENSUS: " << listaUAVs.size() << endl;
                    ConsensusAlgorithm consensus(coordUAV, listaUAVs);
                    int resp = 0;
                    if(listaUAVs.size() > 0){
                        resp = consensus.run();
                        cout << "[CONSENSUS]: CONSENSUS EXECUTADO! RESPOSTA: " << resp << endl;
                    }

                    vector<Collision> vecCollisions = consensus.getCollisions();
                    //Se o consensus detectar possível colisão
                    if(vecCollisions.size() > 0){
                        cout << "[CONSENSUS]: possível colisões para tratar!" << endl;
                        cout << "Colisões: " << consensus.getNumberOfCollisions() << endl;

                        for (int i = 0; i < vecCollisions.size(); i++){
                            cout << "UAV a colidir: " << vecCollisions[i].getUAV().getID() << " escapar por: "
                                        << vecCollisions[i].getUAVCase() << endl;
                            //inativarUAV(this->uav.getID());
                            consensusStage[uav.getID()] = 1;
                            //addEscapeCoordinate(Coordinate(lastPosition.x, lastPosition.y, lastPosition.z+500));
                            Coordinate ce = consensus.escapeCoordinate();
                            addEscapeCoordinate(ce);
                            cout << "Target position " << targetPosition << endl;
                            cout << "Target escapeCoordinate " << castCoordinateToCoord(ce) << endl;

                            int j = vecCollisions[i].getUAV().getID();
                            cout << "[CONSENSUS]:[U" << uav.getID() << "] COORDENADAS VINDAS DO UAV" << j << " PARA: " << uav.getID();

                            Coordinate us = vecCollisions[i].getCoordinate();
                            cout << " x: " << us.getX();
                            cout << " y: " << us.getY();
                            cout << " z: " << us.getZ() <<
                            " tipo: " << vecCollisions[i].getUAVCase() << endl;

                            cout << "[CONSENSUS]:[U" << uav.getID() << "] COORDENADAS REAIS UAV" << j << " PARA: " << uav.getID();
                            cout << " x: " << position[j].getX();
                            cout << " y: " << position[j].getY();
                            cout << " z: " << position[j].getZ() <<
                            " tipo: " << vecCollisions[i].getUAVCase() << endl;

                            //inativarUAV(j);


                            //Agora eu tenho que enviar cada colisão pros UAVs lá

                            //inativarUAV(vecCollisions[0].getUAV().getID());

                            ModuleMessage mm(strdup("collision"), 244, 1);
                            mm.setSource(uav.getID());
                            mm.setDestination(vecCollisions[i].getUAV().getID());
                            Collision c(resp, this->uav);
                            c.setCoordinate(castCoordToCoordinate(position[this->uav.getID()]));
                            mm.setCollision(c);
                            sendMessageToModule(mm);
                        }

                    }

                }

                //if(verificarSeTemUAVProximo() == true)
                    //ChamarAlgoritmoDoConsenso Que lá vai chamar função de adicionarCoordenadaNoVetorParaEvitarColisao
                cout << "VERIFICAR SE TEM UAV PRÓXIMO! > ";

                executeTask(task);
            }
        }
        //Se não tiver tarefas para fazer
        else{
            cout << "não tem tarefa! " << uav.getID() << endl;
            UAVMysCommunication u;
            u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
            u.setSelfID(this->uav.getID());
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getSelfID());
            u.dispatchMessage(msg);
            if(++myStage == 1){
                Coord p(uniform(10, 25), uniform(10, 25), uniform(50, 80));
                targetPosition = assignCoordinate(p);
            }else{
                targetPosition = assignCoordinate(getRandomPosition());
                cout << "ALEATORIA X" << myStage << endl;
            }
        }

        //Default values to assign for UAV movements
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->rescueDataAndStoreVariables();
}

//Base Method
void UAVMobility::move() {
    if(bateria[uav.getID()] < 0.005 && ativo[uav.getID()]){
        cout << "inativou inesperadamente!" << endl;
        inativarUAV(uav.getID());
    }
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueDataAndStoreVariables();

}

//Base Method
double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

//Auxiliar Method
void UAVMobility::initStoppedUAVs() {
    if (uav.getID() == 0) {
        ativarUAV(uav.getID());
        UAVRole = UAVMobility::ROLE_LEADER;
    } else {
        inativarUAV(uav.getID());
        UAVRole = UAVMobility::ROLE_SLAVE;
    }
}

//Auxiliar Method
void UAVMobility::initAuxiliarTasksVariables() {
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        waypoints[i] = 0;
    }
}

//Auxiliar Method
J UAVMobility::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

//Auxiliar Method
void UAVMobility::rescueDataAndStoreVariables(){
    position[uav.getID()] = lastPosition;
    UAVStatus us = this->uav.getStatus();
    us.setLocation(castCoordToCoordinate(position[uav.getID()]));
    this->uav.setStatus(us);
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
}

//Auxiliar Method
Coord UAVMobility::splittedGoTo(int j){
    //Fazer um if para posição inicial para cada UAV (0,0,0) para UAV[0]
    Coord c(0,0,0);

    if(waypoints[uav.getID()] < splitGoTos[uav.getID()].size()){
        //Initial status
        if(waypoints[uav.getID()] == 0){
            //Update status
            tasksVector[uav.getID()][j].setStatus(Task::STARTED);
        }

        c = this->castCoordinateToCoord(splitGoTos[uav.getID()][waypoints[uav.getID()]]);
        cout << "Split: " << waypoints[uav.getID()] << endl;
        cout << "QTD Splits: " << splitGoTos[uav.getID()].size() << endl;
        cout << "x: " << splitGoTos[uav.getID()][waypoints[uav.getID()]].getX();
        cout << " y: " << splitGoTos[uav.getID()][waypoints[uav.getID()]].getY();
        cout << " z: " << splitGoTos[uav.getID()][waypoints[uav.getID()]].getZ() << endl;
        waypoints[uav.getID()]++;
    }else{
        cout << "Else do splittedGoTo > ";
        if(tasksVector[uav.getID()][j].getStatus() == Task::STARTED){
            //Update status
            tasksVector[uav.getID()][j].setStatus(Task::WAITING_FOR_SIGN);
            inativarUAV(uav.getID());
            cout << "UAV "<<uav.getID()<<" ESPERANDO POR SINAL!" << endl;
            if(tasksVector[uav.getID()][j].getLeader() >= 0){
                ModuleMessage mm(strdup("WAITTING"), 345, uav.getID(), tasksVector[uav.getID()][j].getLeader());
                mm.setTask(tasksVector[uav.getID()][j]);
                mm.setModule(1);
                sendMessageToModule(mm);
            }

            //SAVIO
            //Criar mensagem para UAV Lider indicando que terminou a tarefa...
        }
        else if(tasksVector[uav.getID()][j].getStatus() == Task::SIGNNED){
            //SAVIO AO RECEBER A RESPOSTA DO UAV LIDER, O STATUS DEVE SER ATUALIZADO COMO COMPLETED
            waypoints[uav.getID()] = 0;
            tasksVector[uav.getID()][j].setStatus(Task::COMPLETED);
            cout << "entrou aqui signed." << endl;
        }else{
            cout << "entrou aqui else. Próxima tarefa?" << endl;
            /*if(tasksVector[uav.getID()][j].getStatus() == Task::COMPLETED){
                itera[uav.getID()]++;
            }*/
            //xczxczxc
        }
        c = lastPosition;
    }
    return c;
}

//Auxiliar Method
void UAVMobility::executeTask(int j){
    cout << "ExecuteTask" << endl;
    if(consensusStage[uav.getID()] == 3 && lastPosition == targetPosition){
        consensusStage[uav.getID()] = 4;
    }else if(tasksVector[uav.getID()][j].getType() == Task::GOTO){
        if(waypoints[uav.getID()] == 0){
            TaskAssistant t;
            Coordinate target = tasksVector[uav.getID()][j].getTarget();
            //splitGoTos[uav.getID()] = t.splitCoordinate(target, 25);
            splitGoTos[uav.getID()] = t.splitCoordinateFormation(castCoordToCoordinate(lastPosition), target, 25);
        }

        cout << "SPLITTEDGOTO" << endl;
        targetPosition = assignCoordinate(splittedGoTo(j));
    }else{
        targetPosition = assignCoordinate(this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget()));
        itera[uav.getID()]++;
        cout << "Else executeTask" << endl;
    }
}

void UAVMobility::inativarUAV(int idUAV){
    ativo[idUAV] = false;
    //cout << "Inativando UAV[" << idUAV << "]" << endl;
    //cout << "UAV"<< idUAV << " PAROU EM: " << position[idUAV].getX() << "|" << position[idUAV].getY() << "|" << position[idUAV].getZ() << endl;
}
void UAVMobility::ativarUAV(int idUAV){
    ativo[idUAV] = true;
}

void UAVMobility::addEscapeCoordinate(Coordinate coord){
    auto it = splitGoTos[uav.getID()].begin();
    splitGoTos[uav.getID()].insert(it + waypoints[uav.getID()], coord);
}

void UAVMobility::testeConsensus() {
    if (uav.getID() == 0) {
        //Variáveis para o consensus
        Coordinate uav0(0, 0, 25);
        Coordinate uav1(1, 1, 1);
        Coordinate uav2(2, 2, 2);
        Coordinate uav3(7, 50, 150);
        std::vector<UAV> testeUAVs;
        UAV u1(1);
        u1.setStatus(UAVStatus(uav1));
        UAV u2(2);
        u2.setStatus(UAVStatus(uav2));
        UAV u3(3);
        u3.setStatus(UAVStatus(uav3));
        testeUAVs.push_back(u1);
        testeUAVs.push_back(u2);
        testeUAVs.push_back(u3);
        //Consensus do UAV0
        ConsensusAlgorithm consensus(uav0, testeUAVs);
        consensus.run();
        Collision collisionWithUAV1, collisionWithUAV2;
        cout << "Colisões: " << consensus.getNumberOfCollisions() << endl;
        for (Collision c : consensus.getCollisions()) {
            cout << "UAV a colidir: " << c.getUAV().getID() << " escapar por: "
                    << c.getUAVCase() << endl;
            if (c.getUAV().getID() == 1)
                collisionWithUAV1 = c;
            else if (c.getUAV().getID() == 2)
                collisionWithUAV2 = c;
        }
        cout << "Decisão do UAV0: ANTES " << endl;
        cout << "Minha coordenada UAV0 - X: " << uav0.getX() << " Y: "
                << uav0.getY() << " Z: " << uav0.getZ() << endl;
        Coordinate ec0 = consensus.escapeCoordinate();
        cout << "Escape coordinate  UAV0 - X: " << ec0.getX() << " Y: "
                << ec0.getY() << " Z: " << ec0.getZ() << endl;
        //Instanciando o consensus do outro UAV
        std::vector<UAV> testeUAVs1;
        ConsensusAlgorithm consensus1(uav1, testeUAVs1);
        cout << "Decisão do UAV1: " << endl;
        consensus1.makeDecision(collisionWithUAV1);
        cout << "PERANTE SUGESTÃO DO UAV0: " << collisionWithUAV1.getUAVCase()
                << endl;
        cout << "Minha coordenada - X: " << uav1.getX() << " Y: " << uav1.getY()
                << " Z: " << uav1.getZ() << endl;
        Coordinate ec1 = consensus1.escapeCoordinate();
        cout << "Escape coordinate - X: " << ec1.getX() << " Y: " << ec1.getY()
                << " Z: " << ec1.getZ() << endl;
        std::vector<UAV> testeUAVs2;
        ConsensusAlgorithm consensus2(uav2, testeUAVs2);
        cout << "Decisão do UAV2: " << endl;
        consensus2.makeDecision(collisionWithUAV2);
        cout << "PERANTE SUGESTÃO DO UAV0: " << collisionWithUAV1.getUAVCase()
                << endl;
        cout << "Minha coordenada - X: " << uav2.getX() << " Y: " << uav2.getY()
                << " Z: " << uav2.getZ() << endl;
        Coordinate ec2 = consensus2.escapeCoordinate();
        cout << "Escape coordinate - X: " << ec2.getX() << " Y: " << ec2.getY()
                << " Z: " << ec2.getZ() << endl;
        //O UAV QUE VAI RECEBER A MENSAGEM COM A COLISÃO DEVE FAZER:
        //AO RECEBER A MENSAGEM DE COLISÃO, MANDAR MENSAGEM PARA TODOS OS UAVS PARA SABER SUAS POSIÇÕES
        //RODAR O ALGORITMO DE CONSENSUS.RUN()
        //COM AS VARIÁVEIS CALCULADAS, EXCLUIR A COLISÃO OBVIA COM O UAV QUE RECEBEU A MENSAGME DE COLISÃO
        //RODAR O .MAKEDECISION(COLISAO) E RETORNAR A DECISÃO PRO UAV QUE IA COLIDIR
        //E DEPOIS GETESCAPE() PARA RECEBER A NOVA COORDENADA PARA SEGUIR...
        //resposta do UAV1 para a colisão com UAV0
        Collision collisionWithUAV0;
        for (Collision c : consensus1.getCollisions())
            if (c.getUAV().getID() == 0)
                collisionWithUAV0 = c;
        cout << "Z da Colisão com o UAV0 ? "
                << collisionWithUAV0.getCoordinate().getZ() << endl;
        cout << "Decisão do UAV0: DEPOIS " << endl;
        cout << "Decisão: " << consensus.getDecision() << "|"
                << collisionWithUAV0.getUAVCase() << endl;
        consensus.makeDecision(collisionWithUAV0);
        cout << "Minha coordenada UAV0 - X: " << uav0.getX() << " Y: "
                << uav0.getY() << " Z: " << uav0.getZ() << endl;
        ec0 = consensus.escapeCoordinate();
        cout << "Escape coordinate  UAV0 - X: " << ec0.getX() << " Y: "
                << ec0.getY() << " Z: " << ec0.getZ() << endl;
    }
}

void UAVMobility::verificarMensagens(std::vector<UAV> *listaUAVs){
    //verificando se há mensagem para mim...

    for(auto it = std::begin(msgs[uav.getID()]); it != std::end(msgs[uav.getID()]); it++) {
        if((*it).getModule() == MODULE_ID

                //&& this->uav.getID() == 0 ///REMOVER ESTA LINHA DEPOIS

                && (*it).getCode() == 235){ //SÓ UAV0 ESTÁ OLHANDO AS MENSAGENS... CONSEQUENTEMENTE OS OUTROS NÃO ESTÃO FAZENDO O CONSENSUS AUTOMÁTICO RODAR
            //cout << "[U" << uav.getID() << "] COORDENADAS VINDAS DO UAV" << (*it).getSource() << " PARA: " << (*it).getDestination();

            UAVStatus us = (*it).getStatus();
            /*cout << " x: " << us.getLocationX();
            cout << " y: " << us.getLocationY();
            cout << " z: " << us.getLocationZ() <<
            " tipo: " << (*it).getCode() << endl;*/

            consensusStage[uav.getID()] = 2; //Atualizou coordenadas

            /*cout << "[U" << uav.getID() << "] COORDENADAS REAIS UAV" << (*it).getSource() << " PARA: " << (*it).getDestination();
            cout << " x: " << position[(*it).getSource()].getX();
            cout << " y: " << position[(*it).getSource()].getY();
            cout << " z: " << position[(*it).getSource()].getZ() <<
            " tipo: " << (*it).getCode() << endl;*/

            //if(this->uav.getID() == 0){
                UAV u((*it).getSource());
                u.setStatus(us);
                listaUAVs->push_back(u);
            //}

            //Consensus do UAV0
            msgs[uav.getID()].erase(it);
            it = it - 1;

            //Se entrar aqui, então eu tenho uma lista de mensagens com as distâncias.
            //Com isso eu crio um UAV para cada mensagem e passo a coordenada da mensagem
            //Verifico se há possível colisão para cada UAV
            //Chamo o algoritmo do consenso se houver possível colisão
        }else if((*it).getModule() == MODULE_ID || (*it).getCode() == 244){
            //cout << "MENSAGEM DE COLISÃO RECEBIDA EM: " << this->uav.getID() << " DE: " <<(*it).getSource() << endl;
            ConsensusAlgorithm consensus(castCoordToCoordinate(position[this->uav.getID()]), *listaUAVs);
            //consensus.run();
            Collision collision = (*it).getCollision();
            //cout << "Decisão do UAV1: " << endl;
            consensus.makeDecision(collision);
            //cout << "PERANTE SUGESTÃO DO UAV0: " << collision.getUAVCase() << endl;
            //cout << "Coordenada collision: " << castCoordinateToCoord(collision.getCoordinate()) << endl;
            //cout << "Minha coordenada: " << position[this->uav.getID()] << endl;
            Coordinate ec = consensus.escapeCoordinate();
            //cout << "Escape coordinate - X: " << ec.getX() << " Y: " << ec.getY()
            //        << " Z: " << ec.getZ() << endl;
            addEscapeCoordinate(ec);
            msgs[uav.getID()].erase(it);
            it = it - 1;
            /*addEscapeCoordinate(ec);
            msgs[uav.getID()].erase(it);
            it = it - 1;*/
        }
    }
}
void UAVMobility::sendMessageToModule(ModuleMessage mm){
    bool passouChecagem = true;
    for(ModuleMessage moduleMessage : msgs[uav.getID()]){
        if(mm.getSource() == moduleMessage.getSource()
                && mm.getDestination() == moduleMessage.getDestination()
                && mm.getCode() == moduleMessage.getCode()
                && strcmp(mm.getMsg(), moduleMessage.getMsg()) == 0
                && mm.getModule() == moduleMessage.getModule()){
            passouChecagem = false;
            cout << "MENSAGEM REPETIDA!" << endl;
        }
    }

    if(passouChecagem){
        msgs[uav.getID()].push_back(mm);
    }
}

Coord UAVMobility::posicaoAtual(){
    return lastPosition;
}

