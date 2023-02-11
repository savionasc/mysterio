#include "./FlightModule.h"
#include <iostream>

//#include "../../src/mission/DependentTask.h"
//#include "../communication/uav/UAVDispatcher.h"
//#include "../scenarios/MysterioCommander.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
//using namespace mysterio;

//UAVDispatcher uavs[NUMUAVS];

using namespace power;

Define_Module(FlightModule);

FlightModule::FlightModule(){ nextMoveIsWait = false; }

void FlightModule::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);

    //Getting UAV ID
    //uav.setID(getParentModule()->getIndex());

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
}

void FlightModule::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
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

void FlightModule::move() {
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
}

double FlightModule::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}
