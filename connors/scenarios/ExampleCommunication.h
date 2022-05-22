#ifndef MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#define MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#include "../communication/MysCommunication.h"
#include "../status/MysStatusManager.h"

class ExampleCommunication {
public:
    ExampleCommunication(){}
    virtual ~ExampleCommunication(){}
    MysCommunication conn;
    MysStatusManager* status;
};

#endif
