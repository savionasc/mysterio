#include "../../common/msg/Status.h"

using namespace std;
Status::Status() {
}

void Status::setLocation(double locationX, double locationY, double locationZ){
    this->locationX = locationX;
    this->locationY = locationY;
    this->locationZ = locationZ;
}

double Status::getLocationX(){
    return locationX;
}

double Status::getLocationY(){
    return locationY;
}

double Status::getLocationZ(){
    return locationZ;
}

void Status::setVelocity(double velocity){
    this->velocity = velocity;
}

double Status::getVelocity(){
    return velocity;
}

Status::~Status() {
    // TODO Auto-generated destructor stub
}

