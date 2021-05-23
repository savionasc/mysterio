#ifndef MYSTERIO_SRC_STATUS_STATUS_H_
#define MYSTERIO_SRC_STATUS_STATUS_H_
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include <list>
#include <iostream>

class Status {
public:
    virtual Coordinate getUAVLocation(int idUAV) = 0;
    virtual void updateUAVLocation(Coordinate coord, int idUAV) = 0;
    virtual double getUAVVelocity(int idUAV) = 0;
    virtual void updateUAVVelocity(double velocity, int idUAV) = 0;
    virtual int getFlightTime(int idUAV) = 0;
    virtual int CountActiveUAVs() = 0;
    virtual float getBattery(int idUAV) = 0;

    /*ReadStatusFromDB(int idUAV){
     * Repository r;
     * return r.getStatusLocation(idUAV);
     *}*/

    virtual void subscribe(UAV *uav) = 0;
    virtual void requestStatus() = 0;
    virtual std::list<UAV*> getPublishersList() = 0;
    virtual void unsubscribe(UAV *uav) = 0;
};

#endif
