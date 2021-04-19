#ifndef MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_

class UAVCommunication {
public:
    virtual void connectBase() = 0;
    virtual void dispatchMessage(const char *msg) = 0;
    virtual void disconnectBase() = 0;
    //UpdateData(Status data){ }
    //Location extends Status{ }
    //uav.UpdateData(location);
    //uav.UpdateData(bateria);
    //uav.UpdateData(velocidade);
    //uav.UpdateData(tempoDeVoo);
};

#endif
