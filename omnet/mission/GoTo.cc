#include "GoTo.h"

GoTo::GoTo() {
    Coordinate position(-1,-1,-1);
    this->initialPosition = position;
}
<<<<<<< HEAD

=======
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c
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

void GoTo::assignTask(int idUAV, Coordinate target){
    this->target = target;
    this->idUAV = idUAV;
}

void GoTo::assignTask(int idUAV, Command command, int *args[]){

}

<<<<<<< HEAD
=======

>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c
bool GoTo::isComplete(){
    return false;
}
