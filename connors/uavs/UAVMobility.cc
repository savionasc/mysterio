#include "../uavs/UAVMobility.h"
#include <iostream>

#include "../../src/mission/DependentTask.h"
#include "../scenarios/MysterioCommander.h"
#include "../communication/uav/UAVDispatcher.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

double velocidade[NUMUAVS];
float bateria[NUMUAVS];
double tempoVoo[NUMUAVS];
bool ativo[NUMUAVS];
int itera[NUMUAVS];

int step = 0; //this variable forces terminate current "task" of uav
//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
int waypoints[NUMUAVS];

using namespace power;

Define_Module(UAVMobility);

/* O comunication possui o mobility, então já pega os dados diretamente */
/* Caso o mobility precise pegar algo do communication, passar via param */
/* Via param: getParentModule()->par("savio").setIntValue(9); e get também */

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    /*int x = getParentModule()->par("savio");
    cout << "SAVIOOOOO" << x << endl;
    getParentModule()->par("savio").setIntValue(9);
    x = getParentModule()->par("savio");
    cout << "SAVIOOOOO" << x << endl;*/
    if(stage == 0){
        uav.setID(getParentModule()->getIndex());
        connUAV.setUAV(&uav);
        connUAV.setUAVTaskList(&tasks);
        cout << "UAV-"<< getParentModule()->getIndex() << "stage: " << stage << endl;
        connUAV.connectBase();
    }

    /*UAVStatus ss = getParentModule()->par("ombus").set;
    ss->setVelocity(5.5);
    cout << " X " << ((UAVStatus) getParentModule()->par("ombus")).getVelocity() << endl;*/


    //getParentModule()->
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        waypoints[NUMUAVS] = 0;
    }

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade[uav.getID()] = par("speed").operator double();
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
    this->rescueDataAndStoreVariables();
}

void UAVMobility::setTargetPosition() {
    /*if(uav.getID() == 1){
        cSimulation *currentSimulation = getSimulation();
        inet::UAVMobility *com = nullptr;
        //inet::UAVMobility*
        com = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath("MysterioCommunication.UAV[0].mobility"));
        cout << com->getModuleType() << endl;
        //(omnetpp::cDynamicModuleType)mysterio.connors.uavs.UAVMobility
        cout << "[mobility-uav-"<< uav.getID() <<"] Pegou cabron de: " << com->uav.getID() << " Cabron: " << com->cabron << endl;
    }*/
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
// VELHA
//        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){ //if there are tasks not performed
        if( tasks.size() != itera[uav.getID()] && tasks.size() > 0){ //if there are tasks not performed
            int task = itera[uav.getID()];
// VELHA
//            if(tasksVector[uav.getID()][task].getStatus() == 2){ //finalizando
//                tasksVector[uav.getID()][task].setComplete();
            if(tasks[task].getStatus() == 2){ //finalizando
                tasks[task].setComplete();

                //enviando mensagem de finalizada


                NetworkConfigurations ntc;
                // VELHA
                //ntc.setIdSocket(uavs[uav.getID()].getSocketCode());
                ntc.setIdSocket(connUAV.getSocketCode());
                this->uav.setNetworkConfigurations(ntc);
                UAVDispatcher u;
                u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
                u.setUAV(&uav);
                TaskMessage msg;
                msg.setCode(273);
                msg.setSource(u.getUAV()->getID());
                // VELHA
                //msg.setTask(tasksVector[uav.getID()][task]);
                msg.setTask(tasks[task]);
                u.dispatchTaskMessage(msg);

                //next task
                itera[uav.getID()]++;

            }else{
                executeTask(task);
            }
        }else{
            UAVDispatcher u;
            u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
            u.setUAV(&uav);
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getUAV()->getID());
            u.dispatchMessage(msg);
            targetPosition = getRandomPosition();
            if(u.getUAV()->getID() == 1){
                u.disconnectBase();
            }
        }
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->rescueDataAndStoreVariables();
}

void UAVMobility::move() {
    /*if(uav.getID() == 0){
        cout << uav.getID() << " Movendo: " << cabron << endl;
        cout << uav.getID() << " Vindo diretamento da classe: " << connUAV.changeCabrito() << endl;
        cout << uav.getID() << " Agora é: " << cabron << endl;
        int random = 1+(rand() % 100);
        cout << uav.getID() << " Mudei aqui no mobility pra: " << random << endl;
        cabron = random;
        cout << uav.getID() << " Prova: " << cabron << endl;
        cout << uav.getID() << " Prova no conn: " << connUAV.getCabrito() << endl;

        cout << "Agora vamos mexer com listas..." << endl;
        cout << "mob-size inicial : " << tarefinhas.size() << endl;
        cout << "conn-size inicial : " << connUAV.listSize() << endl;
        UAV u(3);
        Coordinate c1(3, 4, 5);
        Task t1(u, 7, c1);
        connUAV.addNewTask(t1);
        cout << "T1 adicionado!" << endl;
        cout << "mob-size: " << tarefinhas.size() << endl;
        cout << "conn-size: " << connUAV.listSize() << endl;
        cout << "mob-Tarefa-uav: " << tarefinhas[0].getUAV().getID() << " tipo: " << tarefinhas[0].getType() << " coordY? " << tarefinhas[0].getTarget().getY() << endl;
        connUAV.printTask(0);

        Coordinate c2(10, 11, 12);
        Task t2(u, 16, c2);
        connUAV.addNewTask(t2);
        cout << "T2 adicionado!" << endl;
        cout << "mob-size: " << tarefinhas.size() << endl;
        cout << "conn-size: " << connUAV.listSize() << endl;
        cout << "mob-Tarefa-uav: " << tarefinhas[0].getUAV().getID() << " tipo: " << tarefinhas[0].getType() << " coordX? " << tarefinhas[0].getTarget().getX() << endl;
        connUAV.printTask(0);
        cout << "mob-Tarefa-uav: " << tarefinhas[1].getUAV().getID() << " tipo: " << tarefinhas[1].getType() << " coordZ? " << tarefinhas[1].getTarget().getZ() << endl;
        connUAV.printTask(1);
        cout << "removendo 1.." << endl;
        tarefinhas.pop_back();
        cout << "mob-size: " << tarefinhas.size() << endl;
        cout << "conn-size: " << connUAV.listSize() << endl;
        cout << "mob-Tarefa-uav: " << tarefinhas[0].getUAV().getID() << " tipo: " << tarefinhas[0].getType() << " coordX? " << tarefinhas[0].getTarget().getX() << endl;
        connUAV.printTask(0);
        cout << "removendo 0.." << endl;
        connUAV.removeLastTask();
        cout << "mob-size: " << tasks.size() << endl;
        cout << "conn-size: " << connUAV.listSize() << endl;
    }*/
    if(step == 1)
        this->stop();
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueDataAndStoreVariables();
}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

void UAVMobility::rescueDataAndStoreVariables(){
    UAVStatus status;
    status.setLocation(this->castCoordToCoordinate(lastPosition));
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
    this->uav.setStatus(status);
}

Coord UAVMobility::flyAround(int j){
    Coord c;
    // VELHA
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 1){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 2){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 3){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 5){
        //Finalizando task
//        tasksVector[uav.getID()][j].setStatus(2);
        tasks[j].setStatus(2);

        //Próxima coordenada
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());

    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 5) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

Coord UAVMobility::flyAroundSquare(int j){
    Coord c;
    // VELHA
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        //Finalizando task
        if(waypoints[uav.getID()] == 4)
//            tasksVector[uav.getID()][j].setStatus(2);
            tasks[j].setStatus(2);
        //c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 1){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 2){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(waypoints[uav.getID()] == 3){
//        c = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 4) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

void UAVMobility::executeTask(int j){
// VELHA
//    if(tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND){
    if(tasks[j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
//    }else if (tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND_SQUARE){
    }else if (tasks[j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
//        targetPosition = this->CoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        targetPosition = this->CoordinateToCoord(tasks[j].getTarget());
        itera[uav.getID()]++;
    }
}

void UAVMobility::stop(){
    //Finaliza a atividade no Omnet
    nextMoveIsWait = true;
    nextChange = simTime() + 0.1;
    cout << "step" << endl;
    step++;
}
