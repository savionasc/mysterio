#include <mysterio/src/aggregator/SimpleUAV.h>

SimpleUAV::SimpleUAV() {
    // TODO Auto-generated constructor stub
}

SimpleUAV::~SimpleUAV() {
    // TODO Auto-generated destructor stub
}

int SimpleUAV::getID(){
    return this->iD;
}

void SimpleUAV::setID(int id){
    this->iD = id;
}

double SimpleUAV::getXAxis(){
    return this->xAxis;
}

void SimpleUAV::setXAxis(double xAxis){
    this->xAxis = xAxis;
}

double SimpleUAV::getYAxis(){
    return this->yAxis;
}

void SimpleUAV::setYAxis(double yAxis){
    this->yAxis = yAxis;
}

void SimpleUAV::setZAxis(double zAxis){
    this->zAxis = zAxis;
}

double SimpleUAV::getZAxis(){
    return this->zAxis;
}

int SimpleUAV::getTempoDeVoo(){
    return this->tempoDeVoo;
}

void SimpleUAV::setTempoDeVoo(int tempodevoo){
    this->tempoDeVoo = tempodevoo;
}

double SimpleUAV::getVelocidade(){
    return this->velocidade;
}

void SimpleUAV::setVelocidade(double velocidade){
    this->velocidade = velocidade;
}
