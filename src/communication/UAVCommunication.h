#ifndef MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_

class UAVCommunication {
public:
    virtual void dispatch(int msg) = 0;
};

#endif
