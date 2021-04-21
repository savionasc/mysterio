#ifndef MYSTERIO_SRC_UTILS_MESSAGE_H_
#define MYSTERIO_SRC_UTILS_MESSAGE_H_

class Message {
public:
    Message(){ }

    Message(char *msg){ this->msg = msg; }

    virtual ~Message(){ }

    char* getMsg() const { return msg; }

    void setMsg(char* msg) { this->msg = msg; }

private:
    char* msg;
};

#endif
