#ifndef MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#define MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#include <vector>
#include "../mission/MissionPlanner.h"
#include "../../omnet/communication/TaskMessage.h"

//
//#include "../../omnet/communication/SendServerSocket.h"
//#include "../../omnet/communication/ConnServerSocket.cc"
//#include <thread>

#define NUMUAVS 2

//extern int conn[NUMUAVS];

class TaskManager {
public:
    TaskManager(){}
    virtual ~TaskManager(){}

    //return int getCurrentTaskIndex(UAV u)
    //return Task getCurrentTask(UAV u)
    //return string getProgressOfUAV(UAV u)

    //EXPLICATIVO
    //Task(Command command(int *args[]))
    //TaskManager(Task, idUAV)
    virtual void assignTask(Task t, UAV u){ ///TESTAR
        t.uav = u;
        t = this->addTask(t);
        TaskMessage tm;
        tm.destination = u.getID();
        tm.task = t;

        //Falta só enviar pro Drone...

        //Communication c;
        //c.sendMessageToUAV(u.getID(), msg)
        //c.sendTaskMessageToUAVID(u.getID(), tmsg);
    }

    //TaskManager
    virtual void markAsComplete(Coordinate c){//Task
        //Task.uav.get
        //if(c.getX() == target.getX() && c.getY() == target.getY() && c.getZ() == target.getZ())
        //    this->status = COMPLETED;
    }

    Task getTaskByID(UAV u, int id){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        return planner->getTaskByID(u, id);
    }

    Task getTaskByIndex(UAV u, int id){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        return planner->getTaskByIndex(u, id);
    }

    virtual int getNumTasks(UAV u){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        std::vector<Task> v = planner->taskList(u);
        return v.size();
    }

    virtual std::vector<Task> getTaskList(UAV u){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        return planner->taskList(u);
    }

    Task addTask(Task t){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        return planner->addTask(t);
    }

    void setTask(Task t){
        MissionPlanner* planner = MissionPlanner::GetInstance("TASK");
        planner->setTask(t);
    }
};

#endif
