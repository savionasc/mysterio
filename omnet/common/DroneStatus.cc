#include "DroneStatus.h"

DroneStatus::DroneStatus() {
}

void DroneStatus::setLocation(double locationX, double locationY, double locationZ){
    this->locationX = locationX;
    this->locationY = locationY;
    this->locationZ = locationZ;
}

double DroneStatus::getLocationX(){
    return locationX;
}

double DroneStatus::getLocationY(){
    return locationY;
}

double DroneStatus::getLocationZ(){
    return locationZ;
}

void DroneStatus::setVelocity(double velocity){
    this->velocity = velocity;
}

double DroneStatus::getVelocity(){
    return velocity;
}

float DroneStatus::getBattery() {
    return battery;
}

void DroneStatus::setBattery(float battery) {
    this->battery = battery;
}

int DroneStatus::getFlightTime() {
    return flightTime;
}

void DroneStatus::setFlightTime(int flightTime) {
    this->flightTime = flightTime;
}

DroneStatus::~DroneStatus() {
    // TODO Auto-generated destructor stub
}
