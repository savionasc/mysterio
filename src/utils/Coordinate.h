#ifndef MYSTERIO_SRC_UTILS_COORDINATE_H_
#define MYSTERIO_SRC_UTILS_COORDINATE_H_

#include <math.h>

class Coordinate {
public:
    Coordinate(){ }

    Coordinate(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    virtual ~Coordinate(){ }

    float getX() const {
        return x;
    }

    void setX(float x) {
        this->x = x;
    }

    float getY() const {
        return y;
    }

    void setY(float y) {
        this->y = y;
    }

    float getZ() const {
        return z;
    }

    void setZ(float z) {
        this->z = z;
    }

private:
    float x;
    float y;
    float z;
};

#endif
