#include "../../osborn/uavs/UAVMobility.h"

#include <iostream>
#include <queue>

#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/mission/DependentTask.h"
#include "../communication/uav/UAVMysCommunication.h"
#include "TaskAssistant.h"
#include "consensus/ConsensusAlgorithm.h"

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

//Base Method
void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());

    initStoppedUAVs();

    initAuxiliarTasksVariables();

    testeConsensus();

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

        targetPosition = lastPosition;
    }

    //Caso seja lider inativo
    else if(UAVRole == UAVMobility::ROLE_DISABLED && !ativo[uav.getID()]) {
        Coord inicialPosition(10, 0, 10);

        if(targetPosition != inicialPosition){
            targetPosition = inicialPosition;
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
        //Se tiver tarefas para fazer
        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){ //if there are tasks not performed
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

            }else{
                cout << "para executar tarefa > ";
                //TaskMessage tm("checking", 123);

                cout << "[zzz] Mensagens de: " << uav.getID() << endl;


                ModuleMessage mm(strdup("location"), 234, 1);

                mm.setSource(uav.getID());
                msgs[uav.getID()].push_back(mm);

                std::vector<UAV> listaUAVs;
                //verificando se há mensagem para mim...
                for (int i = 0; i < msgs[uav.getID()].size(); i++) {
                    if(msgs[uav.getID()][i].getModule() == MODULE_ID && this->uav.getID() == 0){
                        cout << "[U" << uav.getID() << "] COORDENADAS VINDAS DO UAV" << msgs[uav.getID()][i].getSource() << " PARA: " << msgs[uav.getID()][i].getDestination();
                        UAVStatus us = msgs[uav.getID()][i].getStatus();
                        cout << " x: " << us.getLocationX();
                        cout << " y: " << us.getLocationY();
                        cout << " z: " << us.getLocationZ() <<
                        " tipo: " << msgs[uav.getID()][i].getCode() << endl;

                        if(this->uav.getID() == 0){
                            UAV u(msgs[uav.getID()][i].getSource());
                            u.setStatus(us);
                            listaUAVs.push_back(u);
                        }

                        //Consensus do UAV0
                        msgs[uav.getID()].pop_back();
                        //Se entrar aqui, então eu tenho uma lista de mensagens com as distâncias.
                        //Com isso eu crio um UAV para cada mensagem e passo a coordenada da mensagem
                        //Verifico se há possível colisão para cada UAV
                        //Chamo o algoritmo do consenso se houver possível colisão
                    }
                }

                if(this->uav.getID() == 0){
                    Coordinate c = castCoordToCoordinate(position[this->uav.getID()]);
                    cout << "[CONSENSUS]: NUMERO DE UAVS PARA O CONSENSUS: " << listaUAVs.size() << endl;
                    ConsensusAlgorithm consensus(c, listaUAVs);
                    int resp = consensus.run();
                    cout << "[CONSENSUS]: CONSENSUS EXECUTADO! RESPOSTA: " << resp << endl;

                    if(consensus.getCollisions() > 0){
                        cout << "[CONSENSUS]: possível colisões para tratar!" << endl;

                    }

                }


                //if(verificarSeTemUAVProximo() == true)
                    //ChamarAlgoritmoDoConsenso Que lá vai chamar função de adicionarCoordenadaNoVetorParaEvitarColisao
                cout << "VERIFICAR SE TEM UAV PRÓXIMO! > ";

                executeTask(task);
            //}
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
            if(myStage++ == 1){
                Coord p(uniform(10, 25), uniform(10, 25), uniform(50, 80));
                targetPosition = p;
            }else{
                targetPosition = getRandomPosition();
            }

            if(u.getSelfID() == 1){
                u.disconnectBase();
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
        inativarUAV(uav.getID());
    }
    if(continuoustask){
        //analisarDistanciaOvelha();
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
        }
        else if(tasksVector[uav.getID()][j].getStatus() == Task::SIGNNED){
            waypoints[uav.getID()] = 0;
            tasksVector[uav.getID()][j].setStatus(Task::COMPLETED);
        }
        c = lastPosition;
    }
    return c;
}

//Auxiliar Method
void UAVMobility::executeTask(int j){
    cout << "ExecuteTask" << endl;
    if(tasksVector[uav.getID()][j].getType() == Task::GOTO){
        if(waypoints[uav.getID()] == 0){
            TaskAssistant t;
            Coordinate coord = tasksVector[uav.getID()][j].getTarget();
            splitGoTos[uav.getID()] = t.splitCoordinate(coord);
        }

        cout << "SPLITTEDGOTO" << endl;
        targetPosition = splittedGoTo(j);
    }else{
        targetPosition = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        itera[uav.getID()]++;
        cout << "Else executeTask" << endl;
    }
}

void UAVMobility::inativarUAV(int idUAV){
    ativo[idUAV] = false;
    cout << "Inativando UAV[" << idUAV << "]" << endl;
}
void UAVMobility::ativarUAV(int idUAV){
    ativo[idUAV] = true;
}

void UAVMobility::addEscapeCoordinate(Coordinate coord){
    auto it = splitGoTos[uav.getID()].begin();
    cout << "POSSSSS: " << waypoints[uav.getID()] << endl;
    splitGoTos[uav.getID()].insert(it + waypoints[uav.getID()], coord);
}

