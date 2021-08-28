#ifndef MYSTERIO_OMNET_SINGLETON_H_
#define MYSTERIO_OMNET_SINGLETON_H_

#include <iostream>
#include <thread>
#include <mutex>

class Singleton{
private:
    static Singleton * pinstance_;
    static std::mutex mutex_;

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
    
    std::string value() const{
        return value_;
    } 

    int state() const{
        return state_;
    }
};
#endif
