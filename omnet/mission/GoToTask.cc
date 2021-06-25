#include "GoToTask.h"

GoToTask::GoToTask() {
    Coordinate position(-1,-1,-1);
    this->initialPosition = position;
}
GoToTask::GoToTask(Coordinate targetPosition) {
    this->assignTask(targetPosition);
}

GoToTask::~GoToTask() { }

bool GoToTask::isComplete(Coordinate currentPosition){
    //if(currentPosition == this->target)
    if(currentPosition.getX() == this->target.getX())
        return true;
    return false;
}

Coordinate GoToTask::getTask(){
    return this->target;
}

void GoToTask::assignTask(Coordinate target){
    this->target = target;
}

bool GoToTask::isComplete(){
    return false;
}

void GoToTask::assignTask(){

}
