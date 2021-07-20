#ifndef MYSTERIO_OMNET_MISSION_FLYAROUND_H_
#define MYSTERIO_OMNET_MISSION_FLYAROUND_H_
#include "../../src/mission/Task.h"

class FlyAround : public Task {
public:
    FlyAround();
    FlyAround(int idUAV, Coordinate point, int laps=1);
    virtual ~FlyAround();

    bool isComplete(); //For GoTo Task, Current Coordinate is a parameter necessary to check this
    void assignTask(int idUAV, Command command, int *args[]);
private:
    Coordinate point;
    int lap;
    int laps;
};

#endif
