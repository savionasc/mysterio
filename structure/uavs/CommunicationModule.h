#ifndef MYSTERIO_STRUCTURE_UAVS_COMMUNICATIONMODULE_H_
#define MYSTERIO_STRUCTURE_UAVS_COMMUNICATIONMODULE_H_

#include "inet/common/geometry/common/Coord.h"
#include "UAVMessage_m.h"

#include <iostream>
#include <queue>

using namespace omnetpp;
using namespace std;
using namespace inet;
//using namespace mysterio;

//shared variables

//extern UAVMysCommunication mysterioConnection[NUMUAVS];

namespace inet {

class CommunicationModule : public cSimpleModule {
  protected:
    virtual UAVMessage *generateMessage();
    virtual void forwardMessage(UAVMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int selfID = -2;
    UAVMessage* sendMSGEvt;
};
}

#endif
