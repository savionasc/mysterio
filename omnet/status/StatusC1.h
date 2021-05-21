#ifndef MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#define MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#include "../../src/status/Status.h"
#include "../../src/communication/Communicable.h"
#include "../../src/utils/Message.h"
#include <iostream>
#include <map>

using namespace std;

class StatusC1 : public Status, public Communicable{
public:
    //Communicable
    virtual void onMessageReceive(Message msg){
        if(msg.getCode() == 11){
            cout << "Chegou no Status!" << endl;
            cout << msg.getMsg() << endl;
            enviarResposta(22);
        }
    }

    virtual void enviarResposta(int resp);

    //Status


    //Para contar o tempo, talvez adicionar no subscribe ou no próprio uav
    //clock_t t;
    //t = clock();
    //t = clock() - t;
    //cout << ((double)t)/((CLOCKS_PER_SEC/1000)) << endl;


    virtual void addUAV(UAV uav){
        this->uavs[uav.getID()] = uav; //Substituir
    }
    virtual void removeUAV(int iD){
        uavs.erase(iD); //Substituir
    }
    virtual int getNumUAVs(){
        return this->uavs.size(); //Substituir
    }

    virtual Coordinate getUAVLocation(int idUAV){
        UAV s = pegarUAV(idUAV);
        Coordinate c;
        c.setX(s.getXAxis());
        c.setY(s.getYAxis());
        c.setZ(s.getZAxis());
        return c;
    }

    virtual void updateUAVLocation(Coordinate coord, int idUAV){
        //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
        UAV s = pegarUAV(idUAV);
        s.setXAxis(coord.getX());
        s.setYAxis(coord.getY());
        s.setZAxis(coord.getZ());
        this->uavs[idUAV] = s; //Substituir
    }

    virtual double getUAVVelocity(int idUAV){
        UAV s = pegarUAV(idUAV);
        return s.getVelocidade();
    }

    virtual void updateUAVVelocity(double velocity, int idUAV){
        UAV s = pegarUAV(idUAV);
        s.setVelocidade(velocity);
        this->uavs[idUAV] = s; //Substituir
    }

    virtual int getFlightTime(int idUAV){
        UAV s = pegarUAV(idUAV);
        return s.getFlightTime();
    }

    virtual void updateFlightTime(int time, int idUAV){
        //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
        UAV s = pegarUAV(idUAV);
        s.setFlightTime(time);
        this->uavs[idUAV] = s; //Substituir
    }

    virtual float getBattery(int idUAV){
        UAV s = pegarUAV(idUAV);
        return s.getBattery();
    }

    virtual void updateBattery(float level, int idUAV){
        //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
        UAV s = pegarUAV(idUAV);
        s.setBattery(level);
        this->uavs[idUAV] = s; //Substituir
    }

    virtual int CountActiveUAVs(){
        return numNodes;
    }

    void subscribe(UAV *uav){
        //Criar 2 Repositories
        //Repository -> Que faz só consultas SQL geral e para consultas específicas que retorna UAV
        //Repository.addUAV
        //this->subscribers.push_back(uav);
    }

    void requestStatus(){
        //    CommunicationSocket cs;
        //for (UAV *u : this->getListSubscribers())
            //std::cout << u->getID() << std::endl;
            //               this    uav   msg
            //cs.sendMessage(source, dest, msg);
    }

        std::list<UAV*> getPublishersList(){
        //return this->publishers;
    }

    void unsubscribe(UAV *uav){
        //this->subscribers.remove(uav);
    }

    void saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *status);
    void saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *status);
    Coordinate requestUAVCurrentPosition(int idUAV, StatusC1 *status);
    void saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *status);
    double requestUAVCurrentVelocity(int idUAV, StatusC1 *status);
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
};

#endif
