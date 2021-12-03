#include "MysStatusManager.h"

#include "../communication/MysCommunication.h"

using namespace std;

MysStatusManager* MysStatusManager::mpinstance_{nullptr};
std::mutex MysStatusManager::mutex_;
std::map<int,UAV> MysStatusManager::uavs;

MysStatusManager *MysStatusManager::GetInstance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if (mpinstance_ == nullptr){
        mpinstance_ = new MysStatusManager();
    }
    return mpinstance_;
}

void MysStatusManager::onMessageReceive(Message msg){
    StatusMessage* mMSG = dynamic_cast<StatusMessage*>(&msg);
    cout << mMSG->getMsg() << endl;
    switch (mMSG->getCode()) {
        case LOCATION_STATUS_REQUEST:{
            Coordinate cc = this->getUAVLocation(1);
            cout << "Posicao Geografica recuperada pelo banco." << endl;
            cout << "X: " << cc.getX() << " Y: " << cc.getY() << " Z: " << cc.getZ() << endl;

            //Aqui eu devo enviar para alguém...
            break;
        }
        case LOCATION_STATUS_RESPONSE:{
            Coordinate c(mMSG->getStatus().getLocationX(), mMSG->getStatus().getLocationY(), mMSG->getStatus().getLocationZ());
            this->updateUAVLocation(c, mMSG->getSource());
            break;
        }
        case VELOCITY_STATUS_REQUEST:{
            cout << "Velocidade recuperada pelo banco: " << this->getUAVVelocity(mMSG->getSource()) << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(mMSG->getStatus().getVelocity(), mMSG->getSource());
            break;
        }
        case BATTERY_STATUS_REQUEST:{
            cout << "Bateria recuperada pelo banco: " << this->getBattery(mMSG->getSource()) << endl;
            break;
        }
        case BATTERY_STATUS_RESPONSE:{
            this->updateBattery(mMSG->getStatus().getBattery(), mMSG->getSource());
            break;
        }
        case FLIGHTTIME_STATUS_REQUEST:{
            cout << "Tempo de voo recuperado pelo banco: " << this->getFlightTime(mMSG->getSource()) << endl;
            break;
        }
        case FLIGHTTIME_STATUS_RESPONSE:{
            this->updateFlightTime(mMSG->getStatus().getFlightTime(), mMSG->getSource());
            break;
        }
        default:
            break;
    }
}

void MysStatusManager::onDroneStatusMessageReceive(StatusMessage msg){
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
            Coordinate c(msg.getStatus().getLocationX(), msg.getStatus().getLocationY(), msg.getStatus().getLocationZ());
            this->updateUAVLocation(c, msg.getSource());
            break;
        }
        case VELOCITY_STATUS_REQUEST:{
            cout << "Velocidade recuperada pelo banco: " << this->getUAVVelocity(msg.getSource()) << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(msg.getStatus().getVelocity(), msg.getSource());
            break;
        }
        case BATTERY_STATUS_REQUEST:{
            cout << "Bateria recuperada pelo banco: " << this->getBattery(msg.getSource()) << endl;
            break;
        }
        case BATTERY_STATUS_RESPONSE:{
            this->updateBattery(msg.getStatus().getBattery(), msg.getSource());
            break;
        }
        case FLIGHTTIME_STATUS_REQUEST:{
            cout << "Tempo de voo recuperado pelo banco: " << this->getFlightTime(msg.getSource()) << endl;
            break;
        }
        case FLIGHTTIME_STATUS_RESPONSE:{
            this->updateFlightTime(msg.getStatus().getFlightTime(), msg.getSource());
            break;
        }
        default:
            break;
    }
}

int MysStatusManager::CountActiveUAVs(){
    return numeroDeUAVs;
}

//Getters and updaters
Coordinate MysStatusManager::getUAVLocation(int idUAV){ //Request?
    UAV s = pegarUAV(idUAV);
    Coordinate c(s.getXAxis(), s.getYAxis(), s.getZAxis());

    cout << "Pegando localização do Banco de Dados!" << endl;

    this->r.requestUAVLocation(idUAV);
    return c;
}

void MysStatusManager::updateUAVLocation(Coordinate coord, int idUAV){ //saveUAVCurrentPosition?
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setXAxis(coord.getX());
    s.setYAxis(coord.getY());
    s.setZAxis(coord.getZ());
    this->uavs[idUAV] = s; //Substituir
    this->r.saveUAVLocation(1, coord);
}

double MysStatusManager::getUAVVelocity(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getVelocity(idUAV);
    return s.getVelocidade();
}

void MysStatusManager::updateUAVVelocity(double velocity, int idUAV){
    UAV s = pegarUAV(idUAV);
    s.setVelocidade(velocity);
    this->uavs[idUAV] = s; //Substituir
    this->r.setVelocity(idUAV, velocity);
}

int MysStatusManager::getFlightTime(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getFlightTime(idUAV);
    return s.getFlightTime();
}

void MysStatusManager::updateFlightTime(int time, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setFlightTime(time);
    this->uavs[idUAV] = s; //Substituir
    this->r.setFlightTime(idUAV, time);
}

float MysStatusManager::getBattery(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getBattery(idUAV);
    return s.getBattery();
}

void MysStatusManager::updateBattery(float level, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    s.setBattery(level);
    this->uavs[idUAV] = s; //Substituir
    this->r.setBattery(1, level);
}
