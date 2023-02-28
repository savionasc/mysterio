#include "../uavs/UAVMobility.h"
#include <iostream>

#include "../../src/mission/DependentTask.h"
#include "../communication/uav/UAVDispatcher.h"
#include "../scenarios/MysterioCommander.h"

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

//this variable forces terminate current "task" of uav
//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
int UAVLeader = -1;
int UAVDestino = -1;
int waypoints[NUMUAVS];
int lowbattery[NUMUAVS];
using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    if(stage == 0){
        uav.setID(getParentModule()->getIndex());
        this->currentTask = 0;
        //connUAV.setUAV(&uav);
        connUAV.setSelfID(uav.getID());
        //connUAV.setUAVTaskList(&tasks);
        //connUAV.setCurrentTask(&currentTask);
        cout << "UAV-" << getParentModule()->getIndex() << "stage: " << stage << endl;
        connUAV.connectBase();
    }

    if(uav.getID() != 1){
        ativo[uav.getID()] = true;
    }else{
        ativo[uav.getID()] = false;
    }

    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        waypoints[i] = 0;
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
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else if(!ativo[uav.getID()]) {
        Coord c(10, 5, 10);

        if(targetPosition != c){
            targetPosition = c;
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
        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){
            //if there are tasks not performed
            int task = itera[uav.getID()];

            if(tasksVector[uav.getID()][task].getStatus() == 2){
                //finalizando
                tasksVector[uav.getID()][task].setComplete();

                //enviando mensagem de finalizada

                NetworkConfigurations ntc;
                ntc.setIdSocket(connUAV.getSocketCode());
                this->uav.setNetworkConfigurations(ntc);
                UAVDispatcher u;
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
            UAVDispatcher u;
            u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
            u.setSelfID(this->uav.getID());
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getSelfID());
            u.dispatchMessage(msg);
            if(myStage++ == 0){
                Coord p;
                p.x = uniform(10, 25);
                p.y = uniform(10, 25);
                p.z = uniform(50, 80);
                //Coord c(10, 10, 150);
                targetPosition = p;
            }else{
                targetPosition = getRandomPosition();
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

void UAVMobility::move() {
    if(lowbattery[uav.getID()] == 1){
        //Drenando bateria e repassando tarefa
        cout << "Drenou" << endl;
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
        connUAV.dispatchTaskMessage(msg);
    }
    if(bateria[uav.getID()] < 0.005 && ativo[uav.getID()]){
        ativo[uav.getID()] = false;
    }

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

    //Calculando porcentagem da bateria:
    //float aaa = std::stof(energySto->getResidualEnergyCapacity().str());
    //float bbb = std::stof(energySto->getNominalEnergyCapacity().str());
    //float xa = (aaa / bbb);
    //float res = xa * 100;
    //cout << energySto->getResidualEnergyCapacity() << "/" << energySto->getNominalEnergyCapacity() << "-" << res << endl;
    return energySto->getResidualEnergyCapacity();
}

void UAVMobility::rescueDataAndStoreVariables(){
    position[uav.getID()] = lastPosition;
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
}

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
            connUAV.dispatchTaskMessage(msg);
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

void UAVMobility::executeTask(int j){
    if(tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        itera[uav.getID()]++;
    }

}
