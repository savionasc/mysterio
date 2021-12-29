#ifndef MYSTERIO_SRC_COMMUNICATION_MESSAGE_H_
#define MYSTERIO_SRC_COMMUNICATION_MESSAGE_H_
#include <string.h>

#include "../status/UAVStatus.h"

enum codemessages{
    MESSAGE = 270,
    STATUS_MESSAGE,
    TASK_MESSAGE,
    TASK_COMPLETED_MESSAGE,
    TASK_EMERGENCY_BATTERY_LOW,
    SUBTASK_SUBORDINATE
};

class Message {
public:
    Message(){ }

    Message(char *msg, short int code){
        strcpy(this->msg, msg);
        this->code = code;
    }

    Message(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->code = code;
        this->source = src;
        this->destination = dest;
    }

    virtual ~Message(){ }

    char* getMsg() { return msg; }

    void setMsg(char* msg) { strcpy(this->msg, msg); }

    short int getCode() const { return code; }

    void setCode(short int code) { this->code = code; }

    short int getDestination() const { return destination; }

    void setDestination(short int destination) { this->destination = destination; }

    short int getSource() const { return source; }

    void setSource(short int source) { this->source = source; }

private:
    char msg[1500];
    short int code; //This variable must identify the request type of the message
    short int source;
    short int destination;
    //DroneStatus status;
    //SpecificClass* s = dynamic_cast<SpecificClass*>(dest);
};

#endif
