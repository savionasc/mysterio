#ifndef MYSTERIO_OMNET_EXCOMM1_STATUS_LOCATIONSTATUS_H_
#define MYSTERIO_OMNET_EXCOMM1_STATUS_LOCATIONSTATUS_H_
#include "../communication/CommunicationSocket.h"
#include "../../src/utils/UAV.h"
#include <iostream>
#include <list>
#include "../../src/status/Status.h"

class LocationStatus : public Status, public Communicable {
public:
    LocationStatus();
    virtual ~LocationStatus();
    void subscribe(UAV *uav);
    void requestStatusOnSubscribers();
    void onMessageReceive(int msg){
        printf("O Status de localização recebido foi: %d\n", msg);
    }
    std::list<UAV*> getListSubscribers();
    void stopSubscriber(UAV *uav);
private:
    std::list<UAV*> subscribers;
};

#endif
