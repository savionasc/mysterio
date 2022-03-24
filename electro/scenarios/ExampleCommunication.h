#ifndef MYSTERIO_ELECTRO_EXAMPLECOMMUNICATION_H_
#define MYSTERIO_ELECTRO_EXAMPLECOMMUNICATION_H_
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
