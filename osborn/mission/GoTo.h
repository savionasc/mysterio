#ifndef MYSTERIO_OMNET_MISSION_GOTO_H_
#define MYSTERIO_OMNET_MISSION_GOTO_H_
#include "../../src/mission/Task.h"

class GoTo : public Task {
public:
    GoTo();
    GoTo(int idUAV, Coordinate currentPosition);
    virtual ~GoTo();
    bool isComplete();
    bool isComplete(Coordinate currentPosition);
    Coordinate getTask();
    void assignTask();
    void assignTask(UAV uav, Coordinate target);
    void assignTask(UAV uav, Command command, int *args[]);
    bool started = false;
//protected:
    char objetivo[150]; //Definir um tipo Objetivo
    Coordinate target;
    Coordinate initialPosition;
};

#endif
