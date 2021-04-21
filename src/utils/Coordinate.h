#ifndef MYSTERIO_SRC_UTILS_COORDINATE_H_
#define MYSTERIO_SRC_UTILS_COORDINATE_H_

class Coordinate {
public:
    Coordinate(){ }

    Coordinate(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    virtual ~Coordinate(){ }

    double getX() const {
        return x;
    }

    void setX(double x) {
        this->x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        this->y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        this->z = z;
    }

private:
    double x;
    double y;
    double z;
};

#endif
