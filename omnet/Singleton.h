#ifndef MYSTERIO_OMNET_SINGLETON_H_
#define MYSTERIO_OMNET_SINGLETON_H_
#define NUMUAVS 2

#include "../src/mission/Task.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Singleton{
private:
    static Singleton * pinstance_;
    static std::mutex mutex_;
    static std::vector<Task> tasks[NUMUAVS];

protected:
    Singleton(const std::string value): value_(value) { state_ = 0; }
    ~Singleton() {}
    std::string value_;
    int state_;
    int taskID_;

public:
    Singleton(Singleton &other) = delete;
    void operator=(const Singleton &) = delete;


    static Singleton *GetInstance(const std::string& value);
    
    void SomeBusinessLogic(){
        state_++;
    }
    
    void addTask(Task t){
        t.id = createNewTaskID();
        this->tasks[t.uav.getID()].push_back(t);
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

    int numTasks(UAV u){
        return this->tasks[u.getID()].size();
    }

    int createNewTaskID(){
        return ++taskID_;
    }

    std::string value() const{
        return value_;
    } 

    int state() const{
        return state_;
    }


};
#endif
