#ifndef MYSTERIO_SRC_STATUS_STATUS_H_
#define MYSTERIO_SRC_STATUS_STATUS_H_
#include "../utils/Coordinate.h"
#include <map>

#include "../utils/UAV.h"

class Status {
public:
    //Status();
    //virtual ~Status();
    virtual void addUAV(UAV uav) = 0;
    virtual void removeUAV(int iD) = 0;
    virtual int getNumUAVs() = 0;
    virtual Coordinate getUAVLocation(int idUAV) = 0;
    virtual void setUAVLocation(Coordinate coord, int idUAV) = 0;
    virtual double getUAVVelocity(int idUAV) = 0;
    virtual void setUAVVelocity(double velocity, int idUAV) = 0;
};

#endif
