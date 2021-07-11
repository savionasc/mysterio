#ifndef MYSTERIO_OMNET_MISSION_GOTOTASK_H_
#define MYSTERIO_OMNET_MISSION_GOTOTASK_H_
#include "../../src/mission/Task.h"

class GoToTask : public Task {
public:
    GoToTask();
    GoToTask(int idUAV, Coordinate currentPosition);
    virtual ~GoToTask();
    bool isComplete();
    bool isComplete(Coordinate currentPosition);
    Coordinate getTask();
    void assignTask();
    void assignTask(int idUAV, Coordinate target);
    void assignTask(int idUAV, Command command, int *args[]);
    bool started = false;
//protected:
    char objetivo[150]; //Definir um tipo Objetivo
    Coordinate target;
    Coordinate initialPosition;
};

#endif
