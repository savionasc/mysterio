#include "../uavs/UAVMobility.h"
#include "../mysterio/Example1Communication.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

static int numeroDrones1;

Coord position1[10];
double velocidade1[10];

int UAVLeader1 = -1;
int UAVDestino1 = -1;

using namespace mysterio;

namespace inet {

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    selfID = getParentModule()->getIndex();

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade1[selfID] = par("speed").operator double();
        //velocidade1[selfID] = speedParameter;
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
}

void UAVMobility::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        targetPosition = getRandomPosition();
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        position1[selfID] = lastPosition;
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
}

void UAVMobility::move() {
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

}
