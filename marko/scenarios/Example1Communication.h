#ifndef MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#define MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#include "../communication/MysCommunication.h"
#include "../status/MysStatusManager.h"

class Example1Communication {
public:
    Example1Communication(){}
    virtual ~Example1Communication(){}
    MysCommunication conn;
    MysStatusManager* status;
};

#endif
