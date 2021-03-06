#ifndef MYSTERIO_MARKO_SHEEP_H_
#define MYSTERIO_MARKO_SHEEP_H_

#include "inet/common/INETDefs.h"
#include "inet/mobility/base/LineSegmentsMobilityBase.h"
#include "inet/power/base/EpEnergyStorageBase.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"
#include "../uavs/UAVMessage_m.h"

using namespace omnetpp;
using namespace std;

namespace inet {

class Sheep : public cSimpleModule {
  protected:
    virtual UAVMessage *generateMessage();
    virtual void forwardMessage(UAVMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int ID;
};
}

#endif
