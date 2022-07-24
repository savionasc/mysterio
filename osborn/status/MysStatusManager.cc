#include "../communication/MysCommunication.h"
#include "../../osborn/status/MysStatusManager.h"

#include "../../osborn/communication/MysCommunication.h"

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
        case StatusManager::LOCATION_STATUS_REQUEST:{
            Coordinate cc = this->getUAVLocation(1);
            cout << "Position recovered by database." << endl;
            cout << "X: " << cc.getX() << " Y: " << cc.getY() << " Z: " << cc.getZ() << endl;
            break;
        }
        case StatusManager::LOCATION_STATUS_RESPONSE:{
            Coordinate c(mMSG->getStatus().getLocationX(), mMSG->getStatus().getLocationY(), mMSG->getStatus().getLocationZ());
            this->updateUAVLocation(c, mMSG->getSource());
            break;
        }
        case StatusManager::VELOCITY_STATUS_REQUEST:{
            cout << "Velocity recovered by database: " << this->getUAVVelocity(mMSG->getSource()) << endl;
            break;
        }
        case StatusManager::VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(mMSG->getStatus().getVelocity(), mMSG->getSource());
            break;
        }
        case StatusManager::BATTERY_STATUS_REQUEST:{
            cout << "Battery recovered by database: " << this->getBattery(mMSG->getSource()) << endl;
            break;
        }
        case StatusManager::BATTERY_STATUS_RESPONSE:{
            this->updateBattery(mMSG->getStatus().getBattery(), mMSG->getSource());
            break;
        }
        case StatusManager::FLIGHTTIME_STATUS_REQUEST:{
            cout << "Flight time recovered by database: " << this->getFlightTime(mMSG->getSource()) << endl;
            break;
        }
        case StatusManager::FLIGHTTIME_STATUS_RESPONSE:{
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
        case StatusManager::LOCATION_STATUS_REQUEST:{
            Coordinate cc = this->getUAVLocation(1);
            cout << "Position recovered by database." << endl;
            cout << "X: " << cc.getX() << " Y: " << cc.getY() << " Z: " << cc.getZ() << endl;
            break;
        }
        case StatusManager::LOCATION_STATUS_RESPONSE:{
            Coordinate c(msg.getStatus().getLocationX(), msg.getStatus().getLocationY(), msg.getStatus().getLocationZ());
            this->updateUAVLocation(c, msg.getSource());
            break;
        }
        case StatusManager::VELOCITY_STATUS_REQUEST:{
            cout << "Velocity recovered by database: " << this->getUAVVelocity(msg.getSource()) << endl;
            break;
        }
        case StatusManager::VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(msg.getStatus().getVelocity(), msg.getSource());
            break;
        }
        case StatusManager::BATTERY_STATUS_REQUEST:{
            cout << "Battery recovered by database: " << this->getBattery(msg.getSource()) << endl;
            break;
        }
        case StatusManager::BATTERY_STATUS_RESPONSE:{
            this->updateBattery(msg.getStatus().getBattery(), msg.getSource());
            break;
        }
        case StatusManager::FLIGHTTIME_STATUS_REQUEST:{
            cout << "Flight time recovered by database: " << this->getFlightTime(msg.getSource()) << endl;
            break;
        }
        case StatusManager::FLIGHTTIME_STATUS_RESPONSE:{
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

Coordinate MysStatusManager::getUAVLocation(int idUAV){
    UAV s = pegarUAV(idUAV);
    Coordinate c(s.getStatus().getLocationX(), s.getStatus().getLocationY(), s.getStatus().getLocationZ());

    cout << "Pegando localização do Banco de Dados!" << endl;

    this->r.requestUAVLocation(idUAV);
    return c;
}

void MysStatusManager::updateUAVLocation(Coordinate coord, int idUAV){ //saveUAVCurrentPosition?
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    UAVStatus us = s.getStatus();
    us.setLocation(coord.getX(), coord.getY(), coord.getZ());
    s.setStatus(us);
    //Substituir
    this->uavs[idUAV] = s;
    this->r.saveUAVLocation(1, coord);
}

double MysStatusManager::getUAVVelocity(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getVelocity(idUAV);
    return s.getStatus().getVelocity();
}

void MysStatusManager::updateUAVVelocity(double velocity, int idUAV){
    UAV s = pegarUAV(idUAV);
    UAVStatus us = s.getStatus();
    us.setVelocity(velocity);
    s.setStatus(us);
    //Substituir
    this->uavs[idUAV] = s;
    this->r.setVelocity(idUAV, velocity);
}

int MysStatusManager::getFlightTime(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getFlightTime(idUAV);
    return s.getStatus().getFlightTime();
}

void MysStatusManager::updateFlightTime(int time, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    UAVStatus us = s.getStatus();
    us.setFlightTime(time);
    s.setStatus(us);
    //Substituir
    this->uavs[idUAV] = s;
    this->r.setFlightTime(idUAV, time);
}

float MysStatusManager::getBattery(int idUAV){
    UAV s = pegarUAV(idUAV);
    this->r.getBattery(idUAV);
    return s.getStatus().getBattery();
}

void MysStatusManager::updateBattery(float level, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV s = pegarUAV(idUAV);
    UAVStatus us = s.getStatus();
    us.setBattery(level);
    s.setStatus(us);
    //Substituir
    this->uavs[idUAV] = s;
    this->r.setBattery(1, level);
}

bool MysStatusManager::isAvailable(int idUAV){
    UAV u = pegarUAV(idUAV);
    this->r.isAvailable(idUAV);
    return u.getStatus().isAvailable();
}

void MysStatusManager::updateAvailable(bool available, int idUAV){
    UAV u = pegarUAV(idUAV);
    UAVStatus us = u.getStatus();
    us.setAvailable(available);
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.setAvailable(idUAV, available);
}
