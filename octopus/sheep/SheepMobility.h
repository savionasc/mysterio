#ifndef __MYSTERIO_SHEEPMOBILITY_H
#define __MYSTERIO_SHEEPMOBILITY_H

#include "inet/common/INETDefs.h"
#include "inet/mobility/base/LineSegmentsMobilityBase.h"
#include "inet/power/base/EpEnergyStorageBase.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"

namespace inet {

class INET_API SheepMobility : public LineSegmentsMobilityBase
{
  protected:
    bool nextMoveIsWait;
    cPar *speedParameter = nullptr;
    cPar *waitTimeParameter = nullptr;
    bool hasWaitTime;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    virtual void initialize(int stage) override;

    virtual void setTargetPosition() override;

    virtual Coord getPosition();

    virtual void move() override;

    int ID;

  public:
    SheepMobility();
    virtual double getMaxSpeed() const override;
};

}

#endif
