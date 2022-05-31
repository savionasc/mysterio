#ifndef MYSTERIO_OSBORN_SCENARIOS_MYSTERIOCOMMANDER_H_
#define MYSTERIO_OSBORN_SCENARIOS_MYSTERIOCOMMANDER_H_
#include "../../osborn/communication/MysCommunication.h"
#include "../status/MysStatusManager.h"

class MysterioCommander {
public:
    MysterioCommander(){}
    virtual ~MysterioCommander(){}
    MysCommunication conn;
    MysStatusManager* status;
};

#endif
