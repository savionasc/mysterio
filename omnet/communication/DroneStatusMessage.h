#ifndef MYSTERIO_OMNET_COMMUNICATION_DRONESTATUSMESSAGE_H_
#define MYSTERIO_OMNET_COMMUNICATION_DRONESTATUSMESSAGE_H_
#include "../../src/utils/Message.h"
#include "../status/DroneStatus.h"

class DroneStatusMessage : public Message{
public:
    DroneStatusMessage(){}

    DroneStatusMessage(char *msg, short int code){
        //strcpy(this->msg, msg);
        this->setMsg(msg);
        this->setCode(code);
    }

    DroneStatusMessage(char *msg, short int code, short int src, short int dest){
        //strcpy(this->msg, msg);
        this->setMsg(msg);
        this->setCode(code);
        this->setSource(src);
        this->setDestination(dest);
    }

    virtual ~DroneStatusMessage(){}

    DroneStatus getStatus() {
        return status;
    }

    void setStatus(DroneStatus status) {
        this->status = status;
    }

private:
    DroneStatus status;
    char msg[1500];
    short int code; //This variable must identify the request type of the message
    short int source;
    short int destination;
};
#endif
