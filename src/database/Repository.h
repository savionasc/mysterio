#ifndef MYSTERIO_SRC_DATABASE_REPOSITORY_H_
#define MYSTERIO_SRC_DATABASE_REPOSITORY_H_
#include "../../src/utils/Coordinate.h"

class Repository {
public:
    virtual bool createConnection() = 0;
    virtual void saveUAVLocation(int idUAV, Coordinate coord) = 0;
    virtual Coordinate requestUAVLocation(int idUAV) = 0;
    virtual void destroyConnection() = 0;
};

#endif
