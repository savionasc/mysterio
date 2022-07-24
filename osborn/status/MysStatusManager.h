#ifndef MYSTERIO_OMNET_STATUS_MYSSTATUSMANAGER_H_
#define MYSTERIO_OMNET_STATUS_MYSSTATUSMANAGER_H_
#include "../../src/communication/Communicable.h"
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include "../../src/communication/StatusMessage.h"
#include "../../src/database/RepositoryBase.h"
#include "../../src/status/StatusManager.h"

using namespace std;

class MysStatusManager : public StatusManager, public Communicable{
public:

    //Communicable
    virtual void onMessageReceive(Message msg);
    virtual void onDroneStatusMessageReceive(StatusMessage msg);

    //Status
    virtual int CountActiveUAVs();

    //rescues from the repository
    virtual Coordinate getUAVLocation(int idUAV);
    virtual double getUAVVelocity(int idUAV);
    virtual int getFlightTime(int idUAV);
    virtual float getBattery(int idUAV);
    virtual bool isAvailable(int idUAV);

    //send to repository
    virtual void updateUAVLocation(Coordinate coord, int idUAV);
    virtual void updateUAVVelocity(double velocity, int idUAV);
    virtual void updateFlightTime(int time, int idUAV);
    virtual void updateBattery(float level, int idUAV);
    virtual void updateAvailable(bool available, int idUAV);

    static MysStatusManager *GetInstance();

    MysStatusManager(MysStatusManager &other) = delete;
    void operator=(const MysStatusManager &) = delete;


    int getNumberOfUAVs(){
        return numberOfUAVs;
    }

    void setNumberOfUAVs(int number){
        this->numberOfUAVs = number;
    }

    void addUAV(){
        numberOfUAVs++;
    }

    void addUAV(UAV u){
        uavs[u.getID()] = u;
    }

    int getSize(){
        return uavs.size();
    }

    UAV getUAV(int idUAV){
        return this->accesshUAV(idUAV);
    }

    int getIDSocket(int idUAV){
        return this->accesshUAV(idUAV).getNetworkConfigurations().getIdSocket();
    }

    void setUAV(UAV u){
        uavs[u.getID()] = u;
    }

protected:
    MysStatusManager(){ setNumberOfUAVs(2); r.disablePrints(); }
    int numberOfUAVs;

private:
    int operatingTime;

    //singleton
    static MysStatusManager * mpinstance_;
    static std::mutex mutex_;
    static std::map<int,UAV> uavs;

    UAV accesshUAV(int idUAV){
        UAV u;
        u.setID(-1);
        for (std::map<int, UAV>::iterator it=uavs.begin(); it!=uavs.end(); ++it)
            if(it->first == idUAV)
                return it->second;
        return u;
    }

    RepositoryBase r;
};

#endif
