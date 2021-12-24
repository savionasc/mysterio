#ifndef MYSTERIO_SRC_COMMUNICATION_STATUSMESSAGE_H_
#define MYSTERIO_SRC_COMMUNICATION_STATUSMESSAGE_H_
#include "../status/UAVStatus.h"
#include "Message.h"

class StatusMessage : public Message{
public:
    StatusMessage(){}

    StatusMessage(char *msg, short int code){
        //strcpy(this->msg, msg);
        this->setMsg(msg);
        this->setCode(code);
    }

    StatusMessage(char *msg, short int code, short int src, short int dest){
        //strcpy(this->msg, msg);
        this->setMsg(msg);
        this->setCode(code);
        this->setSource(src);
        this->setDestination(dest);
    }

    virtual ~StatusMessage(){}

    UAVStatus getStatus() {
        return status;
    }

    void setStatus(UAVStatus status) {
        this->status = status;
    }

private:
    UAVStatus status;
    char msg[1500];
    short int code; //This variable must identify the request type of the message
    short int source;
    short int destination;
};
#endif
