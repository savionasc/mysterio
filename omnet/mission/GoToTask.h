#ifndef MYSTERIO_OMNET_MISSION_GOTOTASK_H_
#define MYSTERIO_OMNET_MISSION_GOTOTASK_H_
#include "../../src/mission/Task.h"

class GoToTask : public Task {
public:
    GoToTask();
    virtual ~GoToTask();
    bool isComplete();
    bool isComplete(Coordinate currentPosition);
    void setTask();
    void setTask(Coordinate target);
};

#endif
