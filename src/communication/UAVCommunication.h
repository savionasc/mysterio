#ifndef MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_

#include "Message.h"

class UAVCommunication {
public:
    virtual void connectBase() = 0;
    //virtual void dispatchMessage(const char *msg) = 0;
    virtual void dispatchMessage(Message msg) = 0;
    virtual void disconnectBase() = 0;
    virtual void reconnect() = 0;

    //Ou fazer isso:
        //UpdateData(Status data){ }
        //Location extends Status{ }
        //uav.UpdateData(location);
        //uav.UpdateData(bateria);
        //uav.UpdateData(velocidade);
        //uav.UpdateData(tempoDeVoo);
    //Ou o UAV só se comunica por UAVCommunication.dispatchMessage(msg)
};

#endif
