#ifndef MYSTERIO_OMNET_MISSION_SENDTO_H_
#define MYSTERIO_OMNET_MISSION_SENDTO_H_
#include "../../src/mission/Task.h"

class SendTo : public Task {
public:
    SendTo();
    virtual ~SendTo();
    bool isComplete();
    void assignTask();
};

#endif
