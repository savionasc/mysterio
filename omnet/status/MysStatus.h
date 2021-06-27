#ifndef MYSTERIO_OMNET_STATUS_MYSSTATUS_H_
#define MYSTERIO_OMNET_STATUS_MYSSTATUS_H_
#include "../../src/status/Status.h"
#include "../../src/communication/Communicable.h"
#include "../database/RepositoryMySQL.h"
#include <iostream>
#include <map>

#include "../../src/utils/Codes.h"

using namespace std;

class MysStatus : public Status, public Communicable{
public:
    MysStatus();
    //Communicable
    virtual void onMessageReceive(Message msg);

    //Status
    virtual int CountActiveUAVs();

    virtual Coordinate getUAVLocation(int idUAV);  //rescues from the repository
    virtual void updateUAVLocation(Coordinate coord, int idUAV); //send to repository
    virtual double getUAVVelocity(int idUAV);      //rescues from the repository
    virtual void updateUAVVelocity(double velocity, int idUAV);  //send to repository
    virtual int getFlightTime(int idUAV);          //rescues from the repository
    virtual void updateFlightTime(int time, int idUAV);          //send to repository
    virtual float getBattery(int idUAV);           //rescues from the repository
    virtual void updateBattery(float level, int idUAV);          //send to repository

protected:
    std::map<int,UAV> uavs;
    int tempoDeFuncionamento; //do sistema
    UAV pegarUAV(int idUAV){
        UAV r;
        r.setID(-1);
        for (std::map<int, UAV>::iterator it=uavs.begin(); it!=uavs.end(); ++it)
            if(it->first == idUAV)
                return it->second;
        return r;
    }
    int numNodes;

    RepositoryMySQL r; //Deixar de solicitar diretamente pro banco para pedir pro UAV
};

#endif
