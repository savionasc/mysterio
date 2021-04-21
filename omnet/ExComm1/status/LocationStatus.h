#ifndef MYSTERIO_OMNET_EXCOMM1_STATUS_LOCATIONSTATUS_H_
#define MYSTERIO_OMNET_EXCOMM1_STATUS_LOCATIONSTATUS_H_
#include "../communication/CommunicationSocket.h"
#include "../../../src/status/StatusUpdater.h"
#include "../../../src/utils/UAV.h"
#include <iostream>
#include <list>

class LocationStatus : public StatusUpdater, Communicable {
public:
    LocationStatus();
    virtual ~LocationStatus();
    void subscribe(UAV *uav);
    void requestStatusOnSubscribers();
    void stopSubscriber(UAV *uav);
    void onMessageReceive(int msg){
        printf("O Status de localização recebido foi: %d\n", msg);
    }
private:
    void addSubscriber(UAV uav);
    //UAV subscribers[10];
    std::list<UAV*> subscribers;
    //int numUAVs = 0;
};

#endif
