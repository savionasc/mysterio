#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"

void StatusC1::saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *status){
    Coordinate coord(x, y, z);
    status->updateUAVLocation(coord, idUAV);
}

void StatusC1::saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *status){
    status->updateUAVLocation(coord, idUAV);
}

Coordinate StatusC1::requestUAVCurrentPosition(int idUAV, StatusC1 *status){
    return status->getUAVLocation(idUAV);
}

void StatusC1::saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *status){
    status->updateUAVVelocity(velocity, idUAV);
}

double StatusC1::requestUAVCurrentVelocity(int idUAV, StatusC1 *status){
    return status->getUAVVelocity(idUAV);
}

/*  CommunicationSocket cs; //Enviando Reposta
    Message m(resp, 15);
    cs.sendMessage(&src, &dest, m);  */

StatusC1::StatusC1(){
    this->r.createConnection();
}

void StatusC1::onMessageReceive(Message msg) {
    switch (msg.getCode()) {
        case LOCATION_STATUS_RESPONSE: {
            cout << "Resposta recebida no Status!" << endl;
            cout << msg.getMsg() << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:
            break;
        case BATTERY_STATUS_RESPONSE:
            break;
        case FLIGHTTIME_STATUS_RESPONSE:
            break;
        default:
            break;
    }
}

int StatusC1::CountActiveUAVs(){
    return numNodes;
}

//Getters and updaters
Coordinate StatusC1::getUAVLocation(int idUAV){
    UAV s = pegarUAV(idUAV);
    Coordinate c(s.getXAxis(), s.getYAxis(), s.getZAxis());
    return c;
}

void StatusC1::updateUAVLocation(Coordinate coord, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setXAxis(coord.getX());
    s.setYAxis(coord.getY());
    s.setZAxis(coord.getZ());
    this->uavs[idUAV] = s; //Substituir
}

double StatusC1::getUAVVelocity(int idUAV){
    UAV s = pegarUAV(idUAV);
    return s.getVelocidade();
}

void StatusC1::updateUAVVelocity(double velocity, int idUAV){
    UAV s = pegarUAV(idUAV);
    s.setVelocidade(velocity);
    this->uavs[idUAV] = s; //Substituir
}

int StatusC1::getFlightTime(int idUAV){
    UAV s = pegarUAV(idUAV);
    return s.getFlightTime();
}

void StatusC1::updateFlightTime(int time, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setFlightTime(time);
    this->uavs[idUAV] = s; //Substituir
}

float StatusC1::getBattery(int idUAV){
    UAV s = pegarUAV(idUAV);
    return s.getBattery();
}

void StatusC1::updateBattery(float level, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setBattery(level);
    this->uavs[idUAV] = s; //Substituir
}
