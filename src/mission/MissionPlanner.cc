#include <iostream>
#include "MissionPlanner.h"

using namespace std;

MissionPlanner* MissionPlanner::mpinstance_{nullptr};
std::mutex MissionPlanner::mutex_;
std::vector<Task> MissionPlanner::tasks[NUMUAVS];

MissionPlanner *MissionPlanner::GetInstance(const std::string& value){
    std::lock_guard<std::mutex> lock(mutex_);
    if (mpinstance_ == nullptr){
        mpinstance_ = new MissionPlanner(value);
    }
    return mpinstance_;
}
