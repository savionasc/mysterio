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

public:
    Singleton(Singleton &other) = delete;
    void operator=(const Singleton &) = delete;


    static Singleton *GetInstance(const std::string& value);
    
    void SomeBusinessLogic(){
        state_++;
    }
    
    void addTask(Task t){
        this->tasks[t.uav.getID()].push_back(t);
    }

    Task getTask(UAV u, int t){
        return this->tasks[u.getID()][t];
    }

    int numTasks(UAV u){
        return this->tasks[u.getID()].size();
    }

    std::string value() const{
        return value_;
    } 

    int state() const{
        return state_;
    }
};
#endif
