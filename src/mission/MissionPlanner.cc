#include <iostream>
#include "MissionPlanner.h"

using namespace std;

MissionPlanner* MissionPlanner::mpinstance_{nullptr};
std::mutex MissionPlanner::mutex_;
std::vector<std::vector<Task>> MissionPlanner::tasks;

MissionPlanner *MissionPlanner::GetInstance(const int uavs){
    std::lock_guard<std::mutex> lock(mutex_);
    if (mpinstance_ == nullptr){
        mpinstance_ = new MissionPlanner(uavs);
    }
    return mpinstance_;
}
