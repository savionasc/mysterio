#include "MysStatus.h"

#include "../communication/CommunicationSocket.h"

/*  CommunicationSocket cs; //Enviando Reposta
    Message m(resp, 15);
    cs.sendMessage(&src, &dest, m);  */

MysStatus::MysStatus(){
    this->r.createConnection();
}

void MysStatus::onMessageReceive(Message msg){
    cout << "Resposta recebida no Status!" << endl;
    cout << msg.getMsg() << endl;
    switch (msg.getCode()) {
        case LOCATION_STATUS_REQUEST:{
            Coordinate cc = this->getUAVLocation(1);
            cout << "Posicao Geografica recuperada pelo banco." << endl;
            cout << "X: " << cc.getX() << " Y: " << cc.getY() << " Z: " << cc.getZ() << endl;

            //Aqui eu devo enviar para alguém...
            break;
        }
        case LOCATION_STATUS_RESPONSE:{
            Coordinate c(10, 11, 12);
            this->updateUAVLocation(c, 1);
            break;
        }
        case VELOCITY_STATUS_REQUEST:{
            cout << "Velocidade recuperada pelo banco: " << this->getUAVVelocity(1) << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(140, 1);
            break;
        }
        case BATTERY_STATUS_REQUEST:{
            cout << "Bateria recuperada pelo banco: " << this->getBattery(1) << endl;
            break;
        }
        case BATTERY_STATUS_RESPONSE:{
            this->updateBattery(100, 1);
            break;
        }
        case FLIGHTTIME_STATUS_REQUEST:{
            cout << "Tempo de voo recuperado pelo banco: " << this->getFlightTime(1) << endl;
            break;
        }
        case FLIGHTTIME_STATUS_RESPONSE:{
            this->updateFlightTime(120, 1);
            break;
        }
        default:
            break;
    }
}

int MysStatus::CountActiveUAVs(){
    return numNodes;
}

//Getters and updaters
Coordinate MysStatus::getUAVLocation(int idUAV){ //Request?
    UAV s = pegarUAV(idUAV);
    Coordinate c(s.getXAxis(), s.getYAxis(), s.getZAxis());

    cout << "Pegando localização do Banco de Dados!" << endl;

    //this->r.requestStatusInformation();
    this->r.getUAVLocation(idUAV);
    return c;
}

void MysStatus::updateUAVLocation(Coordinate coord, int idUAV){ //saveUAVCurrentPosition?
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setXAxis(coord.getX());
    s.setYAxis(coord.getY());
    s.setZAxis(coord.getZ());
    this->uavs[idUAV] = s; //Substituir
    cout << "Passando localização pro Banco de Dados!" << endl;
    this->r.setUAVLocation(1, coord);
}

double MysStatus::getUAVVelocity(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getVelocity(idUAV);
    return s.getVelocidade();
}

void MysStatus::updateUAVVelocity(double velocity, int idUAV){
    UAV s = pegarUAV(idUAV);
    s.setVelocidade(velocity);
    this->uavs[idUAV] = s; //Substituir
    this->r.setVelocity(idUAV, velocity);
}

int MysStatus::getFlightTime(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getFlightTime(idUAV);
    return s.getFlightTime();
}

void MysStatus::updateFlightTime(int time, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setFlightTime(time);
    this->uavs[idUAV] = s; //Substituir
    this->r.setFlightTime(idUAV, time);
}

float MysStatus::getBattery(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getBattery(idUAV);
    return s.getBattery();
}

void MysStatus::updateBattery(float level, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setBattery(level);
    this->uavs[idUAV] = s; //Substituir
    this->r.setBattery(1, level);
}
