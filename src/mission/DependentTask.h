#ifndef MYSTERIO_SRC_MISSION_DEPENDENTTASK_H_
#define MYSTERIO_SRC_MISSION_DEPENDENTTASK_H_
#include "Task.h"

class DependentTask : public Task {
private:
    Task parentTask;
public:
    DependentTask(){}
    virtual ~DependentTask(){}

    Task getParentTask() {
        return parentTask;
    }

    void setParentTask(Task parentTask) {
        this->parentTask = parentTask;
    }
};

#endif
