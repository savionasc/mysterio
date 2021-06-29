#include "GoToTask.h"

GoToTask::GoToTask() {
    Coordinate position(-1,-1,-1);
    this->initialPosition = position;
}
GoToTask::GoToTask(int idUAV, Coordinate targetPosition) {
    this->assignTask(idUAV, targetPosition);
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

void GoToTask::assignTask(){

}

void GoToTask::assignTask(int idUAV, Coordinate target){
    this->target = target;
    this->idUAV = idUAV;
}

void GoToTask::assignTask(int idUAV, Command command, int *args[]){

}


bool GoToTask::isComplete(){
    return false;
}
