#include "SheepMobility.h"
#include <iostream>

#include "../communication/UAVMysCommunication.h"
#include "../scenarios/Example1Communication.h"
#include "../../src/mission/DependentTask.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

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
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        targetPosition = getRandomPosition();
        //p.y = uniform(10, 25);
        targetPosition.z = 0;

        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
}

void SheepMobility::move() {
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
}

double SheepMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}
