#ifndef MYSTERIO_OMNET_MISSION_MYSTASK_H_
#define MYSTERIO_OMNET_MISSION_MYSTASK_H_
#include "../../src/mission/DependentTask.h"
#include "Formation.h"

class MysTask : public DependentTask{
public:
    MysTask();
    virtual ~MysTask();
    std::vector<Task> formationToTasks(std::vector<Coordinate> positions);
};

#endif
