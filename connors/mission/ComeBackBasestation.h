#ifndef MYSTERIO_OMNET_MISSION_COMEBACKBASESTATION_H_
#define MYSTERIO_OMNET_MISSION_COMEBACKBASESTATION_H_
#include "../../src/mission/Task.h"

class ComeBackBasestation : public Task {
public:
    ComeBackBasestation();
    virtual ~ComeBackBasestation();
    bool isComplete();
    void assignTask();
    void assignTask(UAV uav, Coordinate target); //Mudar para command em vez de void?
    Coordinate base;
};

#endif
