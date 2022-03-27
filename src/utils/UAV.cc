#include "UAV.h"

UAV::UAV() { }

UAV::UAV(int idUAV) { this->idUAV = idUAV; }

UAV::~UAV() { }

int UAV::getID(){
    return this->idUAV;
}

void UAV::setID(int id){
    this->idUAV = id;
}
