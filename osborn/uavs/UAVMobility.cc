#include "../../osborn/uavs/UAVMobility.h"

#include <iostream>
#include <queue>

#include "../../osborn/communication/UAVMysCommunication.h"
#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/mission/DependentTask.h"

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
std::queue<TaskMessage> msgs;
UAVMysCommunication uavs[NUMUAVS];

int waypoints[NUMUAVS];
int lowbattery[NUMUAVS];
using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

//Base Method
void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());

    initStoppedUAVs();

    initAuxiliarTasksVariables();

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
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    }else if(funcao == UAVMobility::ROLE_SLAVE && !ativo[uav.getID()]){
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = true;

//        Coord inicialPosition(500, 500, 500);
        targetPosition = lastPosition;
        std::cout << "mandou posicao aleatória!" << std::endl;
    }else if(funcao == UAVMobility::ROLE_DISABLED && !ativo[uav.getID()]) {

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

    } else {
        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){ //if there are tasks not performed
            std::cout << "TEM TAREFA!" << std::endl;
            int task = itera[uav.getID()];
            //finalizando
            if(tasksVector[uav.getID()][task].getStatus() == 2){
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
                executeTask(task);
            }
        }else{
            UAVMysCommunication u;
            u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
            u.setSelfID(this->uav.getID());
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getSelfID());
            u.dispatchMessage(msg);
            if(myStage++ == 0){
                Coord p(uniform(10, 25), uniform(10, 25), uniform(50, 80));
                targetPosition = p;
            }else{
                targetPosition = getRandomPosition();
                std::cout << "ALEATÓRIA" << std::endl;
            }

            if(u.getSelfID() == 1){
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

//Base Method
void UAVMobility::move() {
    //Drenando bateria e repassando tarefa
    if(lowbattery[uav.getID()] == 1){
        std::cout << "Drenou" << std::endl;
        cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
        SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
        energySto->consumir();
        lowbattery[uav.getID()] = 2;

        TaskMessage msg;
        msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
        msg.setSource(uav.getID());
        tasksVector[uav.getID()][itera[uav.getID()]].setWaypoints(waypoints[uav.getID()]);
        cout << "MEU WAYPOINT: " << tasksVector[uav.getID()][itera[uav.getID()]].getWaypoints() << endl;
        msg.setTask(tasksVector[uav.getID()][itera[uav.getID()]]);
        //targetPosition = getPosit(10, 5, 10);

        //aqui
        //msg.setCoord(this->castCoordToCoordinate(position[uav.getID()]));
        if(tasksVector[uav.getID()][itera[uav.getID()]].getType() == Task::FLY_AROUND_SQUARE){
            waypoints[uav.getID()]--;
            msg.setCoord(castCoordToCoordinate(flyAroundSquare(itera[uav.getID()])));
        }else{
            msg.setCoord(tasksVector[uav.getID()][itera[uav.getID()]].getTarget());
        }
        uavs[uav.getID()].dispatchTaskMessage(msg);
    }
    if(bateria[uav.getID()] < 0.005 && ativo[uav.getID()]){
        ativo[uav.getID()] = false;
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
        ativo[uav.getID()] = true;
        funcao = UAVMobility::ROLE_LEADER;
    } else {
        ativo[uav.getID()] = false;
        funcao = UAVMobility::ROLE_SLAVE;
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
Coord UAVMobility::flyAround(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 1){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 2){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
        if(uav.getID() == 0){
            cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
            SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
            energySto->consumir();
            TaskMessage msg;
            msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
            msg.setSource(uav.getID());
            tasksVector[uav.getID()][itera[uav.getID()]].setWaypoints(waypoints[uav.getID()]+1);
            msg.setTask(tasksVector[uav.getID()][itera[uav.getID()]]);
            //targetPosition = getPosit(10, 5, 10);
            //cout << "GASTOU!" << endl;
            msg.setCoord(this->castCoordToCoordinate(c));
            uavs[uav.getID()].dispatchTaskMessage(msg);
        }
    }else if(waypoints[uav.getID()] == 3){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 5){
        //Finalizando task
        tasksVector[uav.getID()][j].setStatus(2);
        if(uav.getID() == 1){
            ativo[uav.getID()] = false;
        }

        //Próxima coordenada
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());

    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 5) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

//Auxiliar Method
Coord UAVMobility::flyAroundSquare(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        //Finalizando task
        if(waypoints[uav.getID()] == 4){
            tasksVector[uav.getID()][j].setStatus(2);
            if(uav.getID() == 1){
                ativo[uav.getID()] = false;
            }
        }
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 1){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 2){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(waypoints[uav.getID()] == 3){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 4) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

//Auxiliar Method
void UAVMobility::executeTask(int j){
    cout << "Executando tarefa ";
    if(tasksVector[uav.getID()][j].getType() == Task::GOTO){
        cout << "GOTO" << endl;
        targetPosition = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
    }else if(tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND){
        cout << "FLY_AROUND" << endl;
        targetPosition = flyAround(j);
    }else if (tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND_SQUARE){
        cout << "FLY_AROUND_SQUARE" << endl;
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        itera[uav.getID()]++;
    }
}
