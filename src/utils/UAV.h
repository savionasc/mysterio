#ifndef MYSTERIO_SRC_UTILS_UAV_H_
#define MYSTERIO_SRC_UTILS_UAV_H_

#include "../mission/Competence.h"
#include "../status/UAVStatus.h"
#include "HardwareInformation.h"
#include "NetworkConfigurations.h"

class UAV{
public:
    UAV();
    UAV(int idUAV);
    virtual ~UAV();
    virtual int getID();
    virtual void setID(int id);
    //virtual int getIdSocket();
    //virtual void setIdSocket(int id);

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

    NetworkConfigurations getNetworkConfigurations() {
        return networkConfigurations;
    }

    void setNetworkConfigurations(NetworkConfigurations networkConfigurations) {
        this->networkConfigurations = networkConfigurations;
    }

protected:
    int idUAV;

    Competence competence;

    UAVStatus status;

    HardwareInformation hardwareInformation;

    NetworkConfigurations networkConfigurations;
};

#endif
//MYSTERIO_SRC_UTILS_UAV_H_
