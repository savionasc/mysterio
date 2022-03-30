#ifndef MYSTERIO_SRC_STATUS_STATUSMANAGER_H_
#define MYSTERIO_SRC_STATUS_STATUSMANAGER_H_
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include <list>
#include <iostream>

class StatusManager {
public:
    virtual Coordinate getUAVLocation(int idUAV) = 0;
    virtual void updateUAVLocation(Coordinate coord, int idUAV) = 0;
    virtual double getUAVVelocity(int idUAV) = 0;
    virtual void updateUAVVelocity(double velocity, int idUAV) = 0;
    virtual int getFlightTime(int idUAV) = 0;
    virtual void updateFlightTime(int time, int idUAV) = 0;
    virtual float getBattery(int idUAV) = 0;
    virtual void updateBattery(float level, int idUAV) = 0;
    virtual bool isAvailable(int idUAV) = 0;
    virtual void updateAvailable(bool available, int idUAV) = 0;
    virtual int CountActiveUAVs() = 0;
};

#endif
