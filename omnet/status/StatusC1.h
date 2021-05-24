#ifndef MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#define MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#include "../../src/status/Status.h"
#include "../../src/communication/Communicable.h"
#include "../../src/utils/Codes.h"
#include "../../src/utils/Message.h"
#include "../database/RepositoryMySQL.h"
#include <iostream>
#include <map>

using namespace std;

class StatusC1 : public Status, public Communicable{
public:
    StatusC1();
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

    void subscribe(UAV uav){
        //Criar 2 Repositories
        //Repository -> Que faz só consultas SQL geral e para consultas específicas que retorna UAV
        //Repository.addUAV
        //this->subscribers.push_back(uav);
    }

    void requestStatus(){   //cobra os UAVs por Status
        //    CommunicationSocket cs;
        //for (UAV *u : this->getListSubscribers())
            //std::cout << u->getID() << std::endl;
            //               this    uav   msg
            //cs.sendMessage(source, dest, msg);
    }

    std::list<UAV*> getPublishersList(){
        //return this->publishers;
    }

    void unsubscribe(UAV uav){
        //this->subscribers.remove(uav);
    }
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

    RepositoryMySQL r;
};

#endif
