#ifndef MYSTERIO_SRC_COMMUNICATION_STATUS_H_
#define MYSTERIO_SRC_COMMUNICATION_STATUS_H_

class UAVStatus {
public:
    double getLocationX(){
        return locationX;
    }

    double getLocationY(){
        return locationY;
    }

    double getLocationZ(){
        return locationZ;
    }

    void setLocation(double locationX, double locationY, double locationZ){
        this->locationX = locationX;
        this->locationY = locationY;
        this->locationZ = locationZ;
    }

    void setVelocity(double velocity){
        this->velocity = velocity;
    }

    double getVelocity(){
        return velocity;
    }

    float getBattery() {
        return battery;
    }

    void setBattery(float battery) {
        this->battery = battery;
    }

    int getFlightTime() {
        return flightTime;
    }

    void setFlightTime(int flightTime) {
        this->flightTime = flightTime;
    }

private:
    double locationX;
    double locationY;
    double locationZ;
    double velocity;
    float battery;
    int flightTime;
};

#endif
