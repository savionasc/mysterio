#include "../utils/UAV.h"

UAV::UAV() { }

UAV::UAV(int idUAV) { this->iD = idUAV; }

UAV::~UAV() { }

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

float UAV::getBattery() {
    return battery;
}
void UAV::setBattery(float battery) {
    this->battery = battery;
}

int UAV::getFlightTime() {
    return flightTime;
}

void UAV::setFlightTime(int flightTime) {
    this->flightTime = flightTime;
}

/*bool UAV::operator()(UAV const &u1, UAV const &u2){
    if(u1.getID() == u2.getID())
        return true;
    else
        return false;
}*/

void UAV::onMessageReceive(Message msg){

}
