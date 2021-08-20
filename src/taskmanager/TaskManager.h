#ifndef MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#define MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#include <vector>
#define NUMUAVS 2

class TaskManager {
public:
    TaskManager(){}
    virtual ~TaskManager(){}

    //Task container
    std::vector<Task*> base[NUMUAVS];
};

#endif /* MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_ */
