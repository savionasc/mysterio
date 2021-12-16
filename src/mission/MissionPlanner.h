#ifndef MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_
#define MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_

#include "Task.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class MissionPlanner{
private:
    static MissionPlanner * mpinstance_;
    static std::mutex mutex_;
    static std::vector<std::vector<Task>> tasks;

protected:
    //MissionPlanner(const std::string value): value_(value) { taskID_ = 0; uavs = -1; }
    MissionPlanner(const int uavs): numUAVs_(uavs) { taskID_ = 0; createVector(numUAVs_); }
    ~MissionPlanner() {}
    int numUAVs_;
    int taskID_;

public:
    MissionPlanner(MissionPlanner &other) = delete;
    void operator=(const MissionPlanner &) = delete;

    void createVector(int n){
        for (int i = 0; i < n; i++) {
            std::vector<Task> v;
            tasks.push_back(v);
        }
    }

    //static MissionPlanner *GetInstance(const std::string& value);

    static MissionPlanner *GetInstance(const int uavs);

    Task addTask(Task t){
        t.setId(createNewTaskID());
        this->tasks[t.getUAV().getID()].push_back(t);
        return t;
    }

    Task getTaskByIndex(UAV u, int t){
        return this->tasks[u.getID()][t];
    }

    Task getTaskByID(UAV u, int id){
        for (int i = 0; i < this->tasks[u.getID()].size(); i++) {
            if(this->tasks[u.getID()][i].getID() == id){
                return this->tasks[u.getID()][i];
            }
        }
        Task t;
        return t;
    }

    void setTask(Task t){
        for (int i = 0; i < this->tasks[t.getUAV().getID()].size(); i++) {
            if(this->tasks[t.getUAV().getID()][i].getID() == t.getID() ){
                this->tasks[t.getUAV().getID()][i] = t;
            }
        }
    }

    int numTasks(UAV u){
        return this->tasks[u.getID()].size();
    }

    int createNewTaskID(){
        return ++taskID_;
    }

    int numUAVs() const{
        return numUAVs_;
    }

    int taskID() const{
        return taskID_;
    }

    std::vector<Task> taskList(UAV u) const{
        return tasks[u.getID()];
    }

};
#endif
