#include "../uavs/UAVMobility.h"

#include "inet/common/INETMath.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/StringFormat.h"
#include "inet/common/geometry/common/CanvasProjection.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/geometry/common/Quaternion.h"
#include "inet/mobility/contract/IMobility.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <iostream>

#include "../common/msg/MinhaMensagem_m.h"
#include "../common/msg/StatusModule.h"
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
Example1Communication mysterios1;

namespace inet {

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ }

void UAVMobility::initialize(int stage){
    selfID = getParentModule()->getIndex();

    LineSegmentsMobilityBase::initialize(stage);
    EV_TRACE << "initializing MysterioMobility stage " << stage << endl;
    if (stage == INITSTAGE_LOCAL) {
        rad heading = deg(par("initialMovementHeading"));
        rad elevation = deg(par("initialMovementElevation"));
        changeIntervalParameter = &par("changeInterval");
        angleDeltaParameter = &par("angleDelta");
        rotationAxisAngleParameter = &par("rotationAxisAngle");
        speedParameter = &par("speed");
        velocidade1[selfID] = par("speed").operator double();

        quaternion = Quaternion(EulerAngles(heading, -elevation, rad(0)));
        WATCH(quaternion);
    }
}

void UAVMobility::orient(){
    if (faceForward)
        lastOrientation = quaternion;
}

void UAVMobility::setTargetPosition(){
    rad angleDelta = deg(angleDeltaParameter->doubleValue());
    rad rotationAxisAngle = deg(rotationAxisAngleParameter->doubleValue());
    Quaternion dQ = Quaternion(Coord::X_AXIS, rotationAxisAngle.get()) * Quaternion(Coord::Z_AXIS, angleDelta.get());
    quaternion = quaternion * dQ;
    quaternion.normalize();
    Coord direction = quaternion.rotate(Coord::X_AXIS);

    simtime_t nextChangeInterval = *changeIntervalParameter;
    EV_DEBUG << "interval: " << nextChangeInterval << endl;
    sourcePosition = lastPosition;

    //if(targetPositionCommand != nullptr)
        //targetPosition = targetPositionCommand;
    //else
        targetPosition = lastPosition + direction * (*speedParameter) * nextChangeInterval.dbl();

    previousChange = simTime();
    nextChange = previousChange + nextChangeInterval;
}

void UAVMobility::move(){
    simtime_t now = simTime();
    rad dummyAngle;
    if (now == nextChange) {
        lastPosition = targetPosition;
        position1[selfID] = lastPosition;
        handleIfOutside(REFLECT, targetPosition, lastVelocity, dummyAngle, dummyAngle, quaternion);
        EV_INFO << "reached current target position = " << lastPosition << endl;
        setTargetPosition();
        EV_INFO << "new target position = " << targetPosition << ", next change = " << nextChange << endl;
        lastVelocity = (targetPosition - lastPosition) / (nextChange - simTime()).dbl();
        handleIfOutside(REFLECT, targetPosition, lastVelocity, dummyAngle, dummyAngle, quaternion);
    }else if (now > lastUpdate) {
        ASSERT(nextChange == -1 || now < nextChange);
        double alpha = (now - previousChange) / (nextChange - previousChange);
        lastPosition = sourcePosition * (1 - alpha) + targetPosition * alpha;
        handleIfOutside(REFLECT, targetPosition, lastVelocity, dummyAngle, dummyAngle, quaternion);
    }
}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

}
