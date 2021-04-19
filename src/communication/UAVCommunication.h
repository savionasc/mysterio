#ifndef MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_

class UAVCommunication {
public:
    virtual void connectBase() = 0;
    virtual void dispatchMessage(const char *msg) = 0;
};

#endif
