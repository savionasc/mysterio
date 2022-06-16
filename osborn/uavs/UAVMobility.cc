#include "../../osborn/uavs/UAVMobility.h"

#include <iostream>
#include <queue>

#include "../../osborn/communication/UAVMysCommunication.h"
#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/mission/DependentTask.h"
#include "TaskAssistant.h"

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
std::queue<TaskMessage> msgs;
UAVMysCommunication uavs[NUMUAVS];

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

    /*if(uav.getID() == 0 && myStage++ == 0){
        TaskAssistant t;
        Coordinate coord(600, 300, 800);
        splitGoTos[uav.getID()] = t.splitCoordinate(coord);
        cout << "Vai ler" << endl;
        for (int var = 0; var < splitGoTos[uav.getID()].size(); var++) {
            cout << "x: " << splitGoTos[uav.getID()][var].getX();
            cout << " y: " << splitGoTos[uav.getID()][var].getY();
            cout << " z: " << splitGoTos[uav.getID()][var].getZ() << endl;
        }
    }*/

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

//        Coord inicialPosition(500, 500, 500);
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
            std::cout << "TEM TAREFA!" << std::endl;
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
                //Goto para sincronizado
                /*if((tasksVector[uav.getID()][itera[uav.getID()]].getType() == Task::GOTO) && (tasksVector[uav.getID()][itera[uav.getID()]].getStatus() == Task::STARTED)){
                    //nunca entra aqui por enquanto
                    cout << "executando tarefa > ";
                    if(!(true)){
                        cout << "entrou no infinito > ";
                        if(tasksVector[uav.getID()][itera[uav.getID()]].getAssincrono() == 1){
                            //Se é assincrono, esperar que todos se posicionem na posição
                            //Esperar comando para pular para próxima tarefa...
                            cout << "if > ";
                        }
                        cout << "fora > ";
                        //finaliza a tarefa e passa para outra
                        tasksVector[uav.getID()][itera[uav.getID()]].setStatus(Task::COMPLETED);
                        itera[uav.getID()]++;
                    }else{
                        cout << "entrou no else " << uav.getID() << endl;
                        //esperar mais um pouco
                        inativarUAV(uav.getID());
                    }
                }else{*/
                    cout << "para executar tarefa > ";
                    TaskMessage tm("checking", 123);
                    tm.setDestination(0);
                    msgs.push(tm);
                    //if(verificarSeTemUAVProximo() == true)
                        //ChamarAlgoritmoDoConsenso Que lá vai chamar função de adicionarCoordenadaNoVetorParaEvitarColisao 
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

