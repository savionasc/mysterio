#ifndef MYSTERIO_OMNET_COMMON_MYSMESSAGE_H_
#define MYSTERIO_OMNET_COMMON_MYSMESSAGE_H_
#include "../../src/utils/Message.h"

class MysMessage : public Message {
public:
    MysMessage();
    virtual ~MysMessage();

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

    char* getMsg() { return msg; }

    void setMsg(char* msg) { strcpy(this->msg, msg); }

    short int getCode() const { return code; }

    void setCode(short int code) { this->code = code; }

    short int getDestination() const { return destination; }

    void setDestination(short int destination) { this->destination = destination; }

    short int getSource() const { return source; }

    void setSource(short int source) { this->source = source; }
};

#endif
