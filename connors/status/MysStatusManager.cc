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
            cout << "UAV position recovered by database." << endl;
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
            cout << "Velocity recovered by database: " << this->getUAVVelocity(mMSG->getSource()) << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(mMSG->getStatus().getVelocity(), mMSG->getSource());
            break;
        }
        case BATTERY_STATUS_REQUEST:{
            cout << "Battery recovered by database: " << this->getBattery(mMSG->getSource()) << endl;
            break;
        }
        case BATTERY_STATUS_RESPONSE:{
            this->updateBattery(mMSG->getStatus().getBattery(), mMSG->getSource());
            break;
        }
        case FLIGHTTIME_STATUS_REQUEST:{
            cout << "Flight time recovered by database: " << this->getFlightTime(mMSG->getSource()) << endl;
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
            cout << "UAV position recovered by database." << endl;
            cout << "X: " << cc.getX() << " Y: " << cc.getY() << " Z: " << cc.getZ() << endl;

            //Here must to send for a component...
            break;
        }
        case LOCATION_STATUS_RESPONSE:{
            Coordinate c(msg.getStatus().getLocationX(), msg.getStatus().getLocationY(), msg.getStatus().getLocationZ());
            this->updateUAVLocation(c, msg.getSource());
            break;
        }
        case VELOCITY_STATUS_REQUEST:{
            cout << "Velocity recovered by database: " << this->getUAVVelocity(msg.getSource()) << endl;
            break;
        }
        case VELOCITY_STATUS_RESPONSE:{
            this->updateUAVVelocity(msg.getStatus().getVelocity(), msg.getSource());
            break;
        }
        case BATTERY_STATUS_REQUEST:{
            cout << "Battery recovered by database: " << this->getBattery(msg.getSource()) << endl;
            break;
        }
        case BATTERY_STATUS_RESPONSE:{
            this->updateBattery(msg.getStatus().getBattery(), msg.getSource());
            break;
        }
        case FLIGHTTIME_STATUS_REQUEST:{
            cout << "Flight time recovered by database: " << this->getFlightTime(msg.getSource()) << endl;
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

Coordinate MysStatusManager::getUAVLocation(int idUAV){
    //Getters and updaters
    UAV u = getUAV(idUAV);
    UAVStatus us = u.getStatus();
    Coordinate c(us.getLocationX(), us.getLocationY(), us.getLocationZ());

    cout << "Pegando localização do Banco de Dados!" << endl;

    this->r.requestUAVLocation(idUAV);
    return c;
}

void MysStatusManager::updateUAVLocation(Coordinate coord, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV u = getUAV(idUAV);
    UAVStatus us;
    us.setLocation(coord.getX(), coord.getY(), coord.getZ());
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.saveUAVLocation(1, coord);
}

double MysStatusManager::getUAVVelocity(int idUAV){
    UAV u = getUAV(idUAV);
    this->r.getVelocity(idUAV);
    return u.getStatus().getVelocity();
}

void MysStatusManager::updateUAVVelocity(double velocity, int idUAV){
    UAV u = getUAV(idUAV);
    UAVStatus us = u.getStatus();
    us.setVelocity(velocity);
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.setVelocity(idUAV, velocity);
}

int MysStatusManager::getFlightTime(int idUAV){
    UAV u = getUAV(idUAV);
    this->r.getFlightTime(idUAV);
    return u.getStatus().getFlightTime();
}

void MysStatusManager::updateFlightTime(int time, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV u = getUAV(idUAV);
    UAVStatus us = u.getStatus();
    us.setFlightTime(time);
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.setFlightTime(idUAV, time);
}

float MysStatusManager::getBattery(int idUAV){
    UAV u = getUAV(idUAV);
    this->r.getBattery(idUAV);
    return u.getStatus().getBattery();
}

void MysStatusManager::updateBattery(float level, int idUAV){
    //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
    UAV u = getUAV(idUAV);
    UAVStatus us = u.getStatus();
    us.setBattery(level);
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.setBattery(1, level);
}

void MysStatusManager::updateAvailable(bool available, int idUAV){
    UAV u = getUAV(idUAV);
    UAVStatus us = u.getStatus();
    us.setAvailable(available);
    u.setStatus(us);
    //Substituir
    this->uavs[idUAV] = u;
    this->r.setAvailable(idUAV, available);
}

bool MysStatusManager::isAvailable(int idUAV){
    UAV u = getUAV(idUAV);
    this->r.isAvailable(idUAV);
    return u.getStatus().isAvailable();
}

