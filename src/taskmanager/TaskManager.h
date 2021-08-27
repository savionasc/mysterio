#ifndef MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#define MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#include <vector>
#define NUMUAVS 2

class TaskManager {
public:
    TaskManager(){}
    virtual ~TaskManager(){}

    //return int getCurrentTaskIndex(UAV u)
    //return Task getCurrentTask(UAV u)
    //return string getProgressOfUAV(UAV u)

    //Task container
    std::vector<Task*> tasks[NUMUAVS];

    //Task(Command command(int *args[]))
    //TaskManager(Task, idUAV)
    virtual void assignTask(UAV uav, Command command, int *args[]){ //Drone, comando, parametros do comando
        this->uav = uav;
        this->cmd = command;
        //Aqui envia pro UAV
    }

    //TaskManager
    virtual void markAsComplete(Coordinate c){//Task
        //Task.uav.get
        if(c.getX() == target.getX() && c.getY() == target.getY() && c.getZ() == target.getZ())
            this->status = COMPLETED;
    }
};

#endif
