#ifndef MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_
#define MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_
#define NUMUAVS 2

#include "Task.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class MissionPlanner{
private:
    static MissionPlanner * mpinstance_;
    static std::mutex mutex_;
    static std::vector<Task> tasks[NUMUAVS];

protected:
    MissionPlanner(const std::string value): value_(value) { taskID_ = 0; }
    ~MissionPlanner() {}
    std::string value_;
    int taskID_;

public:
    MissionPlanner(MissionPlanner &other) = delete;
    void operator=(const MissionPlanner &) = delete;


    static MissionPlanner *GetInstance(const std::string& value);
    
    Task addTask(Task t){
        t.id = createNewTaskID();
        this->tasks[t.uav.getID()].push_back(t);
        return t;
    }

    Task getTaskByIndex(UAV u, int t){
        return this->tasks[u.getID()][t];
    }

    Task getTaskByID(UAV u, int id){
        for (int i = 0; i < this->tasks[u.getID()].size(); i++) {
            if(this->tasks[u.getID()][i].id == id){
                return this->tasks[u.getID()][i];
            }
        }
        Task t;
        return t;
    }

    void setTask(Task t){
        for (int i = 0; i < this->tasks[t.uav.getID()].size(); i++) {
            if(this->tasks[t.uav.getID()][i].id == t.id ){
                this->tasks[t.uav.getID()][i] = t;
            }
        }
    }

    int numTasks(UAV u){ //Já feito
        return this->tasks[u.getID()].size();
    }

    int createNewTaskID(){
        return ++taskID_;
    }

    std::string value() const{
        return value_;
    } 

    int taskID() const{
        return taskID_;
    }

    std::vector<Task> taskList(UAV u) const{
        return tasks[u.getID()];
    }

};
#endif
