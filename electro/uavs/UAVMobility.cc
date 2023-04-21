#include "../uavs/UAVMobility.h"
#include <iostream>

#include "../../src/mission/DependentTask.h"
#include "../communication/uav/UAVDispatcher.h"
#include "../scenarios/MysterioCommander.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

//this variable forces terminate current "task" of uav
//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    if(stage == 0){
        uav.setID(getParentModule()->getIndex());
        this->currentTask = 0;
        connUAV.setUAV(&uav);
        connUAV.setUAVTaskList(&tasks);
        connUAV.setCurrentTask(&currentTask);
        cout << "UAV-" << getParentModule()->getIndex() << "stage: " << stage << endl;
        connUAV.connectBase();
    }

    UAVStatus us = uav.getStatus();
    if(uav.getID() != 1){
        us.setWorking(true);
    }else{
        us.setWorking(false);
    }
    uav.setStatus(us);

    currentTask = -1;

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
        UAVStatus us = this->uav.getStatus();
        us.setAvailable(true);
        us.setVelocity(par("speed").operator double());
        this->uav.setStatus(us);
    }
    this->rescueDataAndStoreVariables();
}

void UAVMobility::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else if(!this->uav.getStatus().isWorking()) {
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
        if(tasks.size() != currentTask && tasks.size() > 0){
            //if there are tasks not performed
            int task = currentTask;

            if(tasks[task].getStatus() == 2){
                //finalizando
                tasks[task].setComplete();

                //enviando mensagem de finalizada

                NetworkConfigurations ntc;
                ntc.setIdSocket(connUAV.getSocketCode());
                this->uav.setNetworkConfigurations(ntc);
                UAVDispatcher u;
                u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
                u.setUAV(&uav);
                TaskMessage msg;
                msg.setCode(273);
                msg.setSource(u.getUAV()->getID());
                msg.setTask(tasks[task]);
                u.dispatchTaskMessage(msg);

                //next task
                currentTask++;

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
            if(myStage++ == 0){
                Coord p;
                p.x = uniform(10, 25);
                p.y = uniform(10, 25);
                p.z = uniform(50, 80);
                targetPosition = p;
            }else{
                targetPosition = getRandomPosition();
            }

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
    if(this->uav.getStatus().getBattery() < 0.01f && this->uav.getStatus().isAvailable()){
        //Drenando bateria e repassando tarefa
        cout << "Drenou" << endl;
        cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
        SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
        energySto->consumir();
        UAVStatus us = this->uav.getStatus();
        us.setWorking(false);
        us.setAvailable(false);
        this->uav.setStatus(us);

        int task = currentTask;
        TaskMessage msg;
        msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
        msg.setSource(uav.getID());
        cout << "MEU WAYPOINT: " << tasks[task].getWaypoints() << endl;
        msg.setTask(tasks[task]);

        if(tasks[task].getType() == Task::FLY_AROUND_SQUARE){
            tasks[task].setWaypoints(tasks[task].getWaypoints()-1);
            msg.setCoord(castCoordToCoordinate(flyAroundSquare(task)));
        }else{
            msg.setCoord(tasks[task].getTarget());
        }
        connUAV.dispatchTaskMessage(msg);
    }
    checkEnergy();

    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueDataAndStoreVariables();

}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility::getBattery(){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

void UAVMobility::rescueDataAndStoreVariables(){
    UAVStatus uavStatus = this->uav.getStatus();
    uavStatus.setLocation(castCoordToCoordinate(lastPosition));
    uavStatus.setVelocity(speedParameter->doubleValue());
    uavStatus.setBattery(std::stof(getBattery().str()));
    uavStatus.setFlightTime(simTime().dbl());
    this->uav.setStatus(uavStatus);
}

Coord UAVMobility::flyAround(int j){
    Coord c;
    if(tasks[uav.getID()].getWaypoints() == 0 || tasks[uav.getID()].getWaypoints() == 4){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(tasks[uav.getID()].getWaypoints() == 1){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(tasks[uav.getID()].getWaypoints() == 2){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
        if(uav.getID() == 0){
            cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
            SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
            energySto->consumir();
            int task = currentTask;
            TaskMessage msg;
            msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
            msg.setSource(uav.getID());
            tasks[task].setWaypoints(tasks[uav.getID()].getWaypoints()+1);
            msg.setTask(tasks[task]);
            msg.setCoord(this->castCoordToCoordinate(c));
            connUAV.dispatchTaskMessage(msg);
        }
    }else if(tasks[uav.getID()].getWaypoints() == 3){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(tasks[uav.getID()].getWaypoints() == 5){
        //Finalizando task
        tasks[j].setStatus(2);
        if(uav.getID() == 1){
            UAVStatus us = uav.getStatus();
            us.setWorking(false);
            this->uav.setStatus(us);
        }

        //Próxima coordenada
        c = this->castCoordinateToCoord(tasks[j].getTarget());

    }
    int currentWaypoint = (tasks[uav.getID()].getWaypoints() < 5) ? tasks[uav.getID()].getWaypoints()+1 : 0;
    tasks[uav.getID()].setWaypoints(currentWaypoint);
    return c;
}

Coord UAVMobility::flyAroundSquare(int j){
    Coord c;
    if(tasks[uav.getID()].getWaypoints() == 0 || tasks[uav.getID()].getWaypoints() == 4){
        //Finalizando task
        if(tasks[uav.getID()].getWaypoints() == 4){
            tasks[j].setStatus(2);
            if(uav.getID() == 1){
                UAVStatus us = uav.getStatus();
                us.setWorking(false);
                this->uav.setStatus(us);
            }
        }
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(tasks[uav.getID()].getWaypoints() == 1){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(tasks[uav.getID()].getWaypoints() == 2){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(tasks[uav.getID()].getWaypoints() == 3){
        c = this->castCoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    int currentWaypoint = (tasks[uav.getID()].getWaypoints() < 4) ? tasks[uav.getID()].getWaypoints()+1 : 0;
    tasks[uav.getID()].setWaypoints(currentWaypoint);
    return c;
}

void UAVMobility::executeTask(int j){
    if(tasks[j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (tasks[j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->castCoordinateToCoord(tasks[j].getTarget());
        currentTask++;
    }
}

void UAVMobility::checkEnergy() {
    if (uav.getStatus().getBattery() < 0.005 && this->uav.getStatus().isWorking()) {
        UAVStatus us = uav.getStatus();
        us.setWorking(false);
        this->uav.setStatus(us);
    }
}

