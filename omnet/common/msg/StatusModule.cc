#include "StatusModule.h"

using namespace std;
StatusModule::StatusModule() {
}

void StatusModule::setLocation(double locationX, double locationY, double locationZ){
    this->locationX = locationX;
    this->locationY = locationY;
    this->locationZ = locationZ;
}

double StatusModule::getLocationX(){
    return locationX;
}

double StatusModule::getLocationY(){
    return locationY;
}

double StatusModule::getLocationZ(){
    return locationZ;
}

void StatusModule::setVelocity(double velocity){
    this->velocity = velocity;
}

double StatusModule::getVelocity(){
    return velocity;
}

float StatusModule::getBattery() {
    return battery;
}

void StatusModule::setBattery(float battery) {
    this->battery = battery;
}

int StatusModule::getFlightTime() {
    return flightTime;
}

void StatusModule::setFlightTime(int flightTime) {
    this->flightTime = flightTime;
}

StatusModule::~StatusModule() {
    // TODO Auto-generated destructor stub
}

