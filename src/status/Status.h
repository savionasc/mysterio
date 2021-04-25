#ifndef MYSTERIO_SRC_STATUS_STATUS_H_
#define MYSTERIO_SRC_STATUS_STATUS_H_
#include "../utils/Coordinate.h"
#include <map>

#include "../utils/UAV.h"

class Status {
public:
    //Status();
    //virtual ~Status();
    virtual void addUAV(UAV uav) = 0;   //Remover
    virtual void removeUAV(int iD) = 0; //Remover
    virtual int getNumUAVs() = 0;       //Remover
    virtual Coordinate getUAVLocation(int idUAV) = 0;
    virtual void setUAVLocation(Coordinate coord, int idUAV) = 0; //Remover
    virtual double getUAVVelocity(int idUAV) = 0;
    virtual void setUAVVelocity(double velocity, int idUAV) = 0;  //Remover

    virtual void subscribe(UAV *uav) = 0;
    virtual void requestStatus() = 0;
    virtual std::list<UAV*> getPublishersList() = 0;
    virtual void unsubscribe(UAV *uav) = 0;
};

#endif
