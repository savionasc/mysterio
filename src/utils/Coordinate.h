#ifndef MYSTERIO_SRC_UTILS_COORDINATE_H_
#define MYSTERIO_SRC_UTILS_COORDINATE_H_

class Coordinate {
public:
    Coordinate();
    Coordinate(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    virtual ~Coordinate();
    double x;
    double y;
    double z;
};

#endif /* MYSTERIO_SRC_UTILS_COORDINATE_H_ */
