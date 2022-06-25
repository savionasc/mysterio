#ifndef MYSTERIO_SRC_COMMUNICATION_MODULEMESSAGE_H_
#define MYSTERIO_SRC_COMMUNICATION_MODULEMESSAGE_H_
#include "Message.h"

class ModuleMessage : public Message{

public:
    ModuleMessage(){}

    ModuleMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        this->setCode(code);
    }

    ModuleMessage(char *msg, short int code, int module){
        strcpy(this->msg, msg);
        this->setCode(code);
        this->setModule(module);
    }

    ModuleMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->setCode(code);
        this->setSource(src);
        this->setDestination(dest);
    }

    virtual ~ModuleMessage(){}

    char* getMsg() { return msg; }

    void setMsg(char* msg) { strcpy(this->msg, msg); }

    UAVStatus getStatus() {
        return status;
    }

    void setStatus(UAVStatus status) {
        this->status = status;
    }

    Task getTask() {
        return task;
    }

    void setTask(Task task) {
        this->task = task;
    }

    int getModule() {
        return moduleDestination;
    }

    void setModule(int module) {
        this->moduleDestination = module;
    }

private:
    //This variable must identify the request type of the message
    short int code;
    short int source;
    short int destination;
    char msg[1500];
    UAVStatus status;
    Task task;
    int moduleDestination;
};

#endif
