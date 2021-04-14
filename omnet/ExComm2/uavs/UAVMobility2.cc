#include "../../ExComm2/uavs/UAVMobility2.h"

#include <mysterio/src/mysterio/Mysterio.h>

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

#include "../../common/msg/MinhaMensagem_m.h"
#include "../../common/msg/StatusModule.h"
#include "../../ExComm2/mysterio/Example2Communication.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

static int indice2 = -1;
static int numeroDrones2;

Coord position2[10];
double velocidade2[10];

int UAVOrigem2 = -1;
int UAVDestino2 = -1;

using namespace mysterio;
//Mysterio mysterios;
Example2Communication mysterios2;

namespace inet {

Define_Module(UAVMobility2);

UAVMobility2::UAVMobility2(){
}

//Aqui usa só getParentModule()->getIndex();
void UAVMobility2::initialize(int stage){
    selfID = getParentModule()->getIndex();
    //if(stage == 0 && selfID == 0)
        //mysterios = new Example1Communication();
    /*if(stage == 0 && selfID == 0){
        cout << "Iniciando Mysterio!" << endl;
        //mysterio.Mysterio();
        cout << "Passando o numero 2 para mysterio!" << endl;
        mysterios.communication.saveInformationOnAggregator(2);
        cout << "Recuperando informação de mysterio!" << endl;
        cout << "Informação obtida: " << mysterios.communication.requestInformationByAggregator() << endl;
    }*/

    /*if(stage == 0){
        numeroDrones = par("numeroDrones").operator int();
        cout << "Numero de drones: " << numeroDrones << endl;
    }*/

    //cout <<"Indice: " << indice << " ID: " << getId() << endl;

    LineSegmentsMobilityBase::initialize(stage);
    EV_TRACE << "initializing MysterioMobility stage " << stage << endl;
    if (stage == INITSTAGE_LOCAL) {
        rad heading = deg(par("initialMovementHeading"));
        rad elevation = deg(par("initialMovementElevation"));
        changeIntervalParameter = &par("changeInterval");
        angleDeltaParameter = &par("angleDelta");
        rotationAxisAngleParameter = &par("rotationAxisAngle");
        speedParameter = &par("speed");
        velocidade2[selfID] = par("speed").operator double();

        quaternion = Quaternion(EulerAngles(heading, -elevation, rad(0)));
        WATCH(quaternion);
    }
}

void UAVMobility2::orient(){
    if (faceForward)
        lastOrientation = quaternion;
}

void UAVMobility2::setTargetPosition(){
    rad angleDelta = deg(angleDeltaParameter->doubleValue());
    rad rotationAxisAngle = deg(rotationAxisAngleParameter->doubleValue());
    Quaternion dQ = Quaternion(Coord::X_AXIS, rotationAxisAngle.get()) * Quaternion(Coord::Z_AXIS, angleDelta.get());
    quaternion = quaternion * dQ;
    quaternion.normalize();
    Coord direction = quaternion.rotate(Coord::X_AXIS);

    simtime_t nextChangeInterval = *changeIntervalParameter;
    EV_DEBUG << "interval: " << nextChangeInterval << endl;
    sourcePosition = lastPosition;
    targetPosition = lastPosition + direction * (*speedParameter) * nextChangeInterval.dbl();
    previousChange = simTime();
    nextChange = previousChange + nextChangeInterval;
}

void UAVMobility2::move(){
    simtime_t now = simTime();
    rad dummyAngle;
    if (now == nextChange) {
        lastPosition = targetPosition;
        indice2 = selfID;
        position2[selfID] = lastPosition;
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

double UAVMobility2::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

} // namespace inet
