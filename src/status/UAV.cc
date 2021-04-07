#include "../status/UAV.h"

UAV::UAV() {
    // TODO Auto-generated constructor stub
}

UAV::~UAV() {
    // TODO Auto-generated destructor stub
}

int UAV::getID(){
    return this->iD;
}

void UAV::setID(int id){
    this->iD = id;
}

double UAV::getXAxis(){
    return this->xAxis;
}

void UAV::setXAxis(double xAxis){
    this->xAxis = xAxis;
}

double UAV::getYAxis(){
    return this->yAxis;
}

void UAV::setYAxis(double yAxis){
    this->yAxis = yAxis;
}

void UAV::setZAxis(double zAxis){
    this->zAxis = zAxis;
}

double UAV::getZAxis(){
    return this->zAxis;
}

int UAV::getTempoDeVoo(){
    return this->tempoDeVoo;
}

void UAV::setTempoDeVoo(int tempodevoo){
    this->tempoDeVoo = tempodevoo;
}

double UAV::getVelocidade(){
    return this->velocidade;
}

void UAV::setVelocidade(double velocidade){
    this->velocidade = velocidade;
}
