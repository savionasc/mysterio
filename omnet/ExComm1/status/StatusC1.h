#ifndef MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#define MYSTERIO_OMNET_EXCOMM1_STATUS_STATUSC1_H_
#include "../../../src/status/Status.h"
#include "../../../src/communication/Communicable.h"
#include "../../../src/utils/Message.h"

class StatusC1 : public Status, Communicable{
public:
    StatusC1();
    virtual ~StatusC1();
    //Communicable
    virtual void onMessageReceive(Message msg){ //message);
        if(msg.getCode() == 1){
            enviarResposta(22);
        }
    }

    virtual void enviarResposta(int resp);

    //Status

    virtual void addUAV(UAV uav){
        this->uavs[uav.getID()] = uav;
    }
    virtual void removeUAV(int iD){
        uavs.erase(iD);
    }
    virtual int getNumUAVs(){
        return this->uavs.size();
    }
    virtual Coordinate getUAVLocation(int idUAV){
        UAV s = pegarUAV(idUAV);
        Coordinate c;
        c.setX(s.getXAxis());
        c.setY(s.getYAxis());
        c.setZ(s.getZAxis());
        return c;
    }
    virtual void setUAVLocation(Coordinate coord, int idUAV){
        //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
        UAV s = pegarUAV(idUAV);
        s.setXAxis(coord.getX());
        s.setYAxis(coord.getY());
        s.setZAxis(coord.getZ());
        this->uavs[idUAV] = s;
    }
    virtual double getUAVVelocity(int idUAV){
        UAV s = pegarUAV(idUAV);
        return s.getVelocidade();
    }
    virtual void setUAVVelocity(double velocity, int idUAV){
        UAV s = pegarUAV(idUAV);
        s.setVelocidade(velocity);
        this->uavs[idUAV] = s;
    }
protected: //Variáveis que fui criando e melhorando ainda
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
