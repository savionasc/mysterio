#include "../../../quentin/uavs/unified/UAVUnified.h"

#include <iostream>

#include "../../../src/mission/DependentTask.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

using namespace power;

Define_Module(UAVUnified);

UAVUnified::UAVUnified(){ nextMoveIsWait = false; }

void UAVUnified::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    if(stage == 0){
        uav.setID(getParentModule()->getIndex());
        this->currentTask = 0;
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

void UAVUnified::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        targetPosition = getRandomPosition();
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
}

void UAVUnified::move() {
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueDataAndStoreVariables();
}

double UAVUnified::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVUnified::getBattery(){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

void UAVUnified::rescueDataAndStoreVariables(){
    UAVStatus uavStatus = this->uav.getStatus();
    uavStatus.setLocation(this->castCoordToCoordinate(lastPosition));
    uavStatus.setVelocity(speedParameter->doubleValue());
    uavStatus.setBattery(std::stof(getBattery().str()));
    uavStatus.setFlightTime(simTime().dbl());
    this->uav.setStatus(uavStatus);
}

Coord UAVUnified::flyAround(int j){
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
        tasks[j].setStatus(2);
        c = this->CoordinateToCoord(tasks[j].getTarget());
    }
    waypoints = (waypoints < 5) ? waypoints+1 : 0;
    return c;
}

Coord UAVUnified::flyAroundSquare(int j){
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

void UAVUnified::executeTask(int j){
    if(tasks[j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (tasks[j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->CoordinateToCoord(tasks[j].getTarget());
        currentTask++;
    }
}
