#include "GoToTask.h"

GoToTask::GoToTask() { }

GoToTask::~GoToTask() { }

bool GoToTask::isComplete(Coordinate currentPosition){
    //if(currentPosition == this->target)
    if(currentPosition.getX() == this->target.getX())
        return true;
    return false;
}

void GoToTask::setTask(Coordinate target){
    this->target = target;
}

bool GoToTask::isComplete(){
    return false;
}

void GoToTask::setTask(){

}
