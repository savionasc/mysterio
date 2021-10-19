#ifndef MYSTERIO_OMNET_STATUS_MYSSTATUS_H_
#define MYSTERIO_OMNET_STATUS_MYSSTATUS_H_
#include "../../src/status/Status.h"
#include "../../src/communication/Communicable.h"
#include "../communication/DroneStatusMessage.h"
#include "../database/RepositoryMySQL.h"
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

enum codes{
    //300 - UAV Request
        LOCATION_STATUS_REQUEST = 300,
        VELOCITY_STATUS_REQUEST,
        BATTERY_STATUS_REQUEST,
        FLIGHTTIME_STATUS_REQUEST,

        LOCATION_STATUS_RESPONSE = 310,
        VELOCITY_STATUS_RESPONSE,
        BATTERY_STATUS_RESPONSE,
        FLIGHTTIME_STATUS_RESPONSE,
};

class MysStatus : public Status, public Communicable{
public:

    //Communicable
    virtual void onMessageReceive(Message msg);
    virtual void onDroneStatusMessageReceive(DroneStatusMessage msg);

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

    static MysStatus *GetInstance();

    MysStatus(MysStatus &other) = delete;
    void operator=(const MysStatus &) = delete;


    int numeroUAVs(){
        return numeroDeUAVs;
    }

    void setNumeroDeUAVs(int numero){
        this->numeroDeUAVs = numero;
    }

    void addUAV(){
        numeroDeUAVs++;
    }

    void addUAV(UAV u){
        uavs[u.getID()] = u;
    }

    int getSize(){
        return uavs.size();
    }

    UAV getUAV(int idUAV){
        return this->getUAV(idUAV);
    }

    void setUAV(UAV u){
        uavs[u.getID()] = u;
    }

protected:
    MysStatus(){ setNumeroDeUAVs(2); r.disablePrints(); }
    int numeroDeUAVs;

private:
    int tempoDeFuncionamento; //do sistema

    //singleton
    static MysStatus * mpinstance_;
    static std::mutex mutex_;
    static std::map<int,UAV> uavs;

    UAV pegarUAV(int idUAV){
        UAV u;
        u.setID(-1);
        for (std::map<int, UAV>::iterator it=uavs.begin(); it!=uavs.end(); ++it)
            if(it->first == idUAV)
                return it->second;
        return u;
    }

    RepositoryMySQL r; //Deixar de solicitar diretamente pro banco para pedir pro UAV
};

#endif
