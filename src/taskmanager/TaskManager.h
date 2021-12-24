#ifndef MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#define MYSTERIO_SRC_TASKMANAGER_TASKMANAGER_H_
#include <vector>

#include "../communication/TaskMessage.h"
#include "../mission/MissionPlanner.h"

class TaskManager {
public:
    TaskManager(int numUAVs){
        uavs = numUAVs;
    }
    virtual ~TaskManager(){}

    //return int getCurrentTaskIndex(UAV u)
    //return Task getCurrentTask(UAV u)
    //return string getProgressOfUAV(UAV u)

    //EXPLICATIVO
    //Task(Command command(int *args[]))
    //TaskManager(Task, idUAV)
    virtual void assignTask(Task t, UAV u){ ///TESTAR
        t.setUAV(u);
        //t = this->addTask(t);
        TaskMessage tm;
        tm.setDestination(u.getID());
        tm.setTask(t);

        //Falta só enviar pro Drone...

        //CommunicationSocket c;
        //c.sendMessageToUAV(u.getID(), msg)
        //c.sendTaskMessageToUAVID(u.getID(), tmsg);
    }

    //assignTaskPredefinidas(){
    //}

    //TaskManager
    virtual void markAsComplete(Coordinate c){//Task
        //Task.uav.get
        //if(c.getX() == target.getX() && c.getY() == target.getY() && c.getZ() == target.getZ())
        //    this->status = COMPLETED;
    }

    Task getTaskByID(UAV u, int id){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        return planner->getTaskByID(u, id);
    }

    Task getTaskByIndex(UAV u, int id){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        return planner->getTaskByIndex(u, id);
    }

    virtual int getNumTasks(UAV u){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        std::vector<Task> v = planner->taskList(u);
        return v.size();
    }

    virtual std::vector<Task> getTaskList(UAV u){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        return planner->taskList(u);
    }

    Task addTask(Task t){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        return planner->addTask(t);
    }

    void setTask(Task t){
        MissionPlanner* planner = MissionPlanner::GetInstance(uavs);
        planner->setTask(t);
    }
private:
    int uavs;
};

#endif
