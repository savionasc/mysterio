#include "GoTo.h"

GoTo::GoTo() {
    Coordinate position(-1,-1,-1);
    this->initialPosition = position;
}
GoTo::GoTo(int idUAV, Coordinate targetPosition) {
    this->assignTask(idUAV, targetPosition);
}

GoTo::~GoTo() { }

bool GoTo::isComplete(Coordinate currentPosition){
    //if(currentPosition == this->target)
    if(currentPosition.getX() == this->target.getX())
        return true;
    return false;
}

Coordinate GoTo::getTask(){
    return this->target;
}

void GoTo::assignTask(){

}

void GoTo::assignTask(UAV uav, Coordinate target){
    this->target = target;
    this->idUAV = uav.getID();
}

void GoTo::assignTask(UAV uav, Command command, int *args[]){

}

bool GoTo::isComplete(){
    return false;
}
