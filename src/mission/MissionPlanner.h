#ifndef MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_
#define MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_

#include "Task.h"
#include "../utils/Utils.h"
//#include "../../octopus/status/MysStatusManager.h"
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

    std::string value() const{
        return value_;
    } 

    int taskID() const{
        return taskID_;
    }

    std::vector<Task> taskList(UAV u) const{
        return tasks[u.getID()];
    }

//    void assignAllTasks(){
//        char assuntoMSG[10] = "New Task!";
//
//        for (int itUAV = 0; itUAV < NUMUAVS; itUAV++) {
//            MysStatusManager *ms;
//
//            UAV u = ms->getUAV(itUAV);
//
//            int codeMessage = TASK_MESSAGE;
//            for (int itTask = 0; itTask < this->tasks[itUAV].size(); itTask++) {
//                //Enviando tarefa
//                TaskMessage taskMessage(assuntoMSG, TASK_MESSAGE);
//                //taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
//                taskMessage.setTask(this->getTaskByIndex(u, itTask));
//
//                comm.sendTaskMessageToUAV(u.getNetworkConfigurations().getIdSocket(), taskMessage);
//                cout << "ID of assigned task: " << itTask << endl;
//                cout << "ID of UAV: " << u.getID() << endl;
//                cout << "Number of uav tasks: " << t.getNumTasks(u) << endl;
//            }
//        }
//    }

};
#endif
