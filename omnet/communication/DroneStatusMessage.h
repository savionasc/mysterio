#ifndef MYSTERIO_OMNET_COMMUNICATION_DRONESTATUSMESSAGE_H_
#define MYSTERIO_OMNET_COMMUNICATION_DRONESTATUSMESSAGE_H_
#include "../../src/utils/Message.h"
#include "../common/DroneStatus.h"

class DroneStatusMessage : public Message{
public:
    DroneStatusMessage(){}

    DroneStatusMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        this->code = code;
    }

    DroneStatusMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->code = code;
        this->source = src;
        this->destination = dest;
    }

    virtual ~DroneStatusMessage(){}
    DroneStatus status;
    char msg[1500];
};
#endif
