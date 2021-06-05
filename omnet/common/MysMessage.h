#ifndef MYSTERIO_OMNET_COMMON_MYSMESSAGE_H_
#define MYSTERIO_OMNET_COMMON_MYSMESSAGE_H_
#include "../../src/utils/Message.h"

class MysMessage : public Message {
public:
    MysMessage(){ }

    MysMessage(char *msg, short int code){
        strcpy(this->msg, msg);
        //this->msg = msg;
        this->code = code;
    }

    MysMessage(char *msg, short int code, short int src, short int dest){
        strcpy(this->msg, msg);
        this->code = code;
        this->source = src;
        this->destination = dest;
    }

    virtual ~MysMessage(){ }
};

#endif
