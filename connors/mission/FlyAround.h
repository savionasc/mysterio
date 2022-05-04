#ifndef MYSTERIO_OMNET_MISSION_FLYAROUND_H_
#define MYSTERIO_OMNET_MISSION_FLYAROUND_H_
#include "../../src/mission/Task.h"

class FlyAround : public Task {
public:
    FlyAround();
    FlyAround(UAV uav, Coordinate point, int laps=1);
    virtual ~FlyAround();

    //For GoTo Task, Current Coordinate is a parameter necessary to check this
    bool isComplete();
    void assignTask(UAV uav, Command command, int *args[]);
private:
    Coordinate point;
    int lap;
    int laps;
};

#endif
