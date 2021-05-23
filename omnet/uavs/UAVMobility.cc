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

    //Para contar o tempo, talvez adicionar no subscribe ou aqui no próprio uav
    //clock_t t;
    //t = clock();
    //t = clock() - t;
    //cout << ((double)t)/((CLOCKS_PER_SEC/1000)) << endl;

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
    this->setData();
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
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->setData();
}

void UAVMobility::move() {
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->setData();
}

void UAVMobility::setData(){
    position1[selfID] = lastPosition;
    velocidade1[selfID] = speedParameter->doubleValue();
}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

}
