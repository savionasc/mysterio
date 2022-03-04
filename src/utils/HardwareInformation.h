#ifndef MYSTERIO_SRC_UTILS_HARDWAREINFORMATION_H_
#define MYSTERIO_SRC_UTILS_HARDWAREINFORMATION_H_

class HardwareInformation {
public:
    HardwareInformation(){}
    virtual ~HardwareInformation() {}

    int getAltitudeOfOperation() {
        return altitudeOfOperation;
    }

    void setAltitudeOfOperation(int altitudeOfOperation) {
        this->altitudeOfOperation = altitudeOfOperation;
    }

    int getMacAdress() {
        return macAdress;
    }

    void setMacAdress(int macAdress) {
        this->macAdress = macAdress;
    }

    int getMaxAirspeed() {
        return maxAirspeed;
    }

    void setMaxAirspeed(int maxAirspeed) {
        this->maxAirspeed = maxAirspeed;
    }

    int getMaxOutsideAirTemperature() {
        return maxOutsideAirTemperature;
    }

    void setMaxOutsideAirTemperature(int maxOutsideAirTemperature) {
        this->maxOutsideAirTemperature = maxOutsideAirTemperature;
    }

    int getMinOutsideAirTemperature() {
        return minOutsideAirTemperature;
    }

    void setMinOutsideAirTemperature(int minOutsideAirTemperature) {
        this->minOutsideAirTemperature = minOutsideAirTemperature;
    }

    float getPayloadCapacity() {
        return payloadCapacity;
    }

    void setPayloadCapacity(float payloadCapacity) {
        this->payloadCapacity = payloadCapacity;
    }

    float getWeightOfUav() {
        return weightOfUAV;
    }

    void setWeightOfUav(float weightOfUav) {
        weightOfUAV = weightOfUav;
    }

private:
    int macAdress;
    float weightOfUAV;
    float payloadCapacity;
    int altitudeOfOperation;
    int minOutsideAirTemperature;
    int maxOutsideAirTemperature;
    int maxAirspeed;
};

#endif
