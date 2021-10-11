#ifndef MYSTERIO_OMNET_COMMUNICATION_TASKMESSAGE_H_
#define MYSTERIO_OMNET_COMMUNICATION_TASKMESSAGE_H_
#include "../../src/utils/Message.h"
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
