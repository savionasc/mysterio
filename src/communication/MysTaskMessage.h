#ifndef MYSTERIO_SRC_COMMUNICATION_MYSTASKMESSAGE_H_
#define MYSTERIO_SRC_COMMUNICATION_MYSTASKMESSAGE_H_

#include "../../osborn/mission/MysTask.h"
#include "../../src/communication/Message.h"

class MysTaskMessage : public Message{
public:
    MysTaskMessage(){}

    MysTaskMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        this->setCode(code);
    }

    MysTaskMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->setCode(code);
        this->setSource(src);
        this->setDestination(dest);
    }

    virtual ~MysTaskMessage(){}

    Task getMysTask() {
        return task;
    }

    void setMysTask(MysTask task) {
        this->task = task;
    }

    Coordinate getCoord(){
        return coord;
    }

    void setCoord(Coordinate coord) {
        this->coord = coord;
    }

private:
    MysTask task;
    Coordinate coord;
    char msg[1500];
};
#endif

