#ifndef MYSTERIO_SRC_UTILS_MESSAGE_H_
#define MYSTERIO_SRC_UTILS_MESSAGE_H_

class Message {
public:
    Message(){ }

    Message(char *msg, short int code){
        this->msg = msg;
        this->code = code;
    }

    virtual ~Message(){ }

    char* getMsg() const { return msg; }

    void setMsg(char* msg) { this->msg = msg; }

    short int getCode() const { return code; }

    void setCode(short int code) { this->code = code; }

private:
    char* msg;
    short int code; //This variable must identify the request type of the message
};

#endif
