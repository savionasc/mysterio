#ifndef MYSTERIO_OMNET_MISSION_COMEBACKBASESTATION_H_
#define MYSTERIO_OMNET_MISSION_COMEBACKBASESTATION_H_
#include "../../src/mission/Task.h"

class ComeBackBasestation : public Task {
public:
    ComeBackBasestation();
    virtual ~ComeBackBasestation();
    bool isComplete();
    void assignTask();
};

#endif
