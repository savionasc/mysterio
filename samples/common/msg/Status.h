#ifndef MYSTERIO_SRC_COMMUNICATION_STATUS_H_
#define MYSTERIO_SRC_COMMUNICATION_STATUS_H_

#include <omnetpp.h>

class Status {
public:
    Status();
    virtual ~Status();
    void setLocation(double locX, double locY, double locZ);
    double getLocationX();
    double getLocationY();
    double getLocationZ();
    void setVelocity(double velocity);
    double getVelocity();
protected:
    double locationX;
    double locationY;
    double locationZ;
    double velocity;
};

#endif
