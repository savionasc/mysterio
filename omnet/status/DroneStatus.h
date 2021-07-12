#ifndef MYSTERIO_SRC_COMMUNICATION_STATUS_H_
#define MYSTERIO_SRC_COMMUNICATION_STATUS_H_

class DroneStatus {
public:
    DroneStatus();
    virtual ~DroneStatus();
    void setLocation(double locX, double locY, double locZ);
    double getLocationX();
    double getLocationY();
    double getLocationZ();
    void setVelocity(double velocity);
    double getVelocity();
    float getBattery();
    void setBattery(float battery);
    int getFlightTime();
    void setFlightTime(int flightTime);

protected:
    double locationX;
    double locationY;
    double locationZ;
    double velocity;
    float battery;
    int flightTime;
};

#endif
