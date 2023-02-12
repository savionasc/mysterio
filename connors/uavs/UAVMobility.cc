#include "../uavs/UAVMobility.h"
#include <iostream>

#include "../../src/mission/DependentTask.h"
#include "../scenarios/MysterioCommander.h"
#include "../communication/uav/UAVDispatcher.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)

using namespace power;

Define_Module(UAVMobility);

/* O comunication possui o mobility, então já pega os dados diretamente */
/* Caso o mobility precise pegar algo do communication, passar via param */
/* Via param: getParentModule()->par("savio").setIntValue(9); e get também */

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    if(stage == 0){
        uav.setID(getParentModule()->getIndex());
        this->currentTask = 0;
        connUAV.setUAV(&uav);
        connUAV.setUAVTaskList(&tasks);
        connUAV.setCurrentTask(&currentTask);
        cout << "UAV-"<< getParentModule()->getIndex() << "stage: " << stage << endl;
        connUAV.connectBase();
    }

    currentTask = -1;
    waypoints = 0;

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        UAVStatus us = this->uav.getStatus();
        us.setVelocity(par("speed").operator double());
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
    this->rescueDataAndStoreVariables();
}

void UAVMobility::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        if( tasks.size() != currentTask && tasks.size() > 0){
            //if there are tasks not performed        
            int task = currentTask;
            if(tasks[task].getStatus() == 2){ //finalizando
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
    uavStatus.setLocation(this->castCoordToCoordinate(lastPosition));
    uavStatus.setVelocity(speedParameter->doubleValue());
    uavStatus.setBattery(std::stof(getBattery().str()));
    uavStatus.setFlightTime(simTime().dbl());
    this->uav.setStatus(uavStatus);
}

Coord UAVMobility::flyAround(int j){
    Coord c;
    if(waypoints == 0 || waypoints == 4){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(waypoints == 1){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(waypoints == 2){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
    }else if(waypoints == 3){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(waypoints == 5){
        //Finishing task
        tasks[j].setStatus(2);
        c = this->CoordinateToCoord(tasks[j].getTarget());
    }
    waypoints = (waypoints < 5) ? waypoints+1 : 0;
    return c;
}

Coord UAVMobility::flyAroundSquare(int j){
    Coord c;
    if(waypoints == 0 || waypoints == 4){
        //Finishing task
        if(waypoints == 4)
            tasks[j].setStatus(2);
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(waypoints == 1){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(waypoints == 2){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(waypoints == 3){
        c = this->CoordinateToCoord(tasks[j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    waypoints = (waypoints < 4) ? waypoints+1 : 0;
    return c;
}

void UAVMobility::executeTask(int j){
    if(tasks[j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (tasks[j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->CoordinateToCoord(tasks[j].getTarget());
        currentTask++;
    }
}
