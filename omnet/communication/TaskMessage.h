#ifndef MYSTERIO_OMNET_COMMUNICATION_TASKMESSAGE_H_
#define MYSTERIO_OMNET_COMMUNICATION_TASKMESSAGE_H_
#include "../../src/utils/Message.h"
#include "../../src/mission/Task.h"
//#include "../status/DroneStatus.h"

class TaskMessage : public Message{
public:
    TaskMessage(){}

    TaskMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        this->code = code;
    }

    TaskMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->code = code;
        this->source = src;
        this->destination = dest;
    }

    virtual ~TaskMessage(){}
    Task task;
    char msg[1500];
};
#endif
