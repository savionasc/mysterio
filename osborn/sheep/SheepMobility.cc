#include "../sheep/SheepMobility.h"

#include <iostream>

using namespace omnetpp;
using namespace std;
using namespace inet;

bool stopped = false;
Coord c;
Define_Module(SheepMobility);

SheepMobility::SheepMobility(){ nextMoveIsWait = false; }

void SheepMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    ID = getParentModule()->getIndex();

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
}

void SheepMobility::setTargetPosition() {
    if(stopped && c.getX() != lastPosition.getX()){
        targetPosition = c;

        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;

    }else if(stopped){
        nextChange = simTime() + 15;
    }else if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        //targetPosition = getRandomPosition();
        targetPosition = getPosition();

        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
}

void SheepMobility::move(){
    if(c.getX() == 0 && stopped){
        c = lastPosition;
    }
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
}

double SheepMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

Coord SheepMobility::getPosition(){
        Coord p;
        p.x = uniform(constraintAreaMin.x, constraintAreaMax.x) * 0.2;
        p.y = uniform(constraintAreaMin.y, constraintAreaMax.y) * 0.2;
        p.z = 0;
        return p;
}

Coord SheepMobility::posicaoAtual(){
    return lastPosition;
}
