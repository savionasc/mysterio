#include "Singleton.h"

using namespace std;

Singleton* Singleton::pinstance_{nullptr};
std::mutex Singleton::mutex_;
std::vector<Task> Singleton::tasks[NUMUAVS];

Singleton *Singleton::GetInstance(const std::string& value){
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr){
        pinstance_ = new Singleton(value);
    }
    return pinstance_;
}
