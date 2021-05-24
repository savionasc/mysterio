#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"

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
            Coordinate c(10, 11, 12);
            this->updateUAVLocation(c, 1);
            this->getUAVLocation(1);
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
Coordinate StatusC1::getUAVLocation(int idUAV){ //Request?
    UAV s = pegarUAV(idUAV);
    Coordinate c(s.getXAxis(), s.getYAxis(), s.getZAxis());

    cout << "Pegando localização do Banco de Dados!" << endl;
    this->r.getUAVLocation(1);
    return c;
}

void StatusC1::updateUAVLocation(Coordinate coord, int idUAV){ //saveUAVCurrentPosition?
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setXAxis(coord.getX());
    s.setYAxis(coord.getY());
    s.setZAxis(coord.getZ());
    this->uavs[idUAV] = s; //Substituir
    cout << "Passando localização pro Banco de Dados!" << endl;
    this->r.setUAVLocation(1, coord);
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
