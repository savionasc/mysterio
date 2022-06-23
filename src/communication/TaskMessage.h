#ifndef MYSTERIO_SRC_COMMUNICATION_TASKMESSAGE_H_
#define MYSTERIO_SRC_COMMUNICATION_TASKMESSAGE_H_
#include "Message.h"
#include "../../src/mission/Task.h"

class TaskMessage : public Message{
public:
    TaskMessage(){}

    TaskMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        this->setCode(code);
    }

    TaskMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->setCode(code);
        this->setSource(src);
        this->setDestination(dest);
    }

    virtual ~TaskMessage(){}

    char* getMsg() { return msg; }

    void setMsg(char* msg) { strcpy(this->msg, msg); }

    Task getTask() {
        return task;
    }

    void setTask(Task task) {
        this->task = task;
    }

    Coordinate getCoord(){
        return coord;
    }

    void setCoord(Coordinate coord) {
        this->coord = coord;
    }

private:
    Task task;
    Coordinate coord;
    char msg[1500];
};
#endif
