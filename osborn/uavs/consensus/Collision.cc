#include "Collision.h"

Collision::Collision() { }

Collision::Collision(int uavCase, UAV uav){
    this->UAVCase = uavCase;
    this->uav = uav;
}

Collision::~Collision() { }

int Collision::getUAVCase(){
    return UAVCase;
}

void Collision::setUAVCase(int uavCase) {
    this->UAVCase = uavCase;
}

UAV Collision::getUAV(){
    return this->uav;
}

void Collision::setUAV(UAV uav){
    this->uav = uav;
}

Coordinate Collision::getCoordinate(){
    return this->uav.getStatus().getCoordinate();
}

void Collision::setCoordinate(Coordinate coordinate){
    UAVStatus us = this->uav.getStatus();
    us.setLocation(coordinate);
    this->uav.setStatus(us);
}
