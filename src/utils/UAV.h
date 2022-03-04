#ifndef MYSTERIO_SRC_UTILS_UAV_H_
#define MYSTERIO_SRC_UTILS_UAV_H_

#include "../mission/Competence.h"
#include "../status/UAVStatus.h"
#include "HardwareInformation.h"

//#include "../communication/Communicable.h"

//Talvez extender/implementar communicable
class UAV{ //: public Communicable { //Aqui eu pensei que seria um exemplo simples duma instância dum uav, tipo isso
    // MOVER PARA UTILS E MUDAR PARA UAV
public:
    UAV();
    UAV(int idUAV);
    virtual ~UAV();
    virtual int getID();
    virtual void setID(int id);
    virtual int getIdSocket();
    virtual void setIdSocket(int id);
    //virtual double getXAxis();
    //virtual void setXAxis(double xAxis);
    //virtual double getYAxis();
    //virtual void setYAxis(double yAxis);
    //virtual double getZAxis();
    //virtual void setZAxis(double zAxis);
    //virtual int getTempoDeVoo();
    //virtual void setTempoDeVoo(int tempodevoo);
    //virtual double getVelocidade();
    //virtual void setVelocidade(double velocidade);

    //Communicable
    //void onMessageReceive(Message msg);

    //virtual float getBattery();
    //virtual void setBattery(float battery);
    //virtual int getFlightTime();
    //virtual void setFlightTime(int flightTime);

    UAVStatus getStatus() {
        return status;
    }

    virtual void setStatus(UAVStatus status) {
        this->status = status;
    }

    HardwareInformation getHardwareInformation() {
        return hardwareInformation;
    }

    void setHardwareInformation(HardwareInformation hardwareInformation) {
        this->hardwareInformation = hardwareInformation;
    }

protected:
    int idUAV;
    int idSocket;
    //int tempoDeVoo;
    //double xAxis;
    //double yAxis;
    //double zAxis;
    //double velocidade;
    //float battery;
    //int flightTime;

    Competence competence;

    UAVStatus status;

    HardwareInformation hardwareInformation;
    //bateria
    //disponivel
};

#endif /* MYSTERIO_SRC_UTILS_UAV_H_ */
