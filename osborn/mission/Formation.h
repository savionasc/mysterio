#ifndef MYSTERIO_OSBORN_MISSION_FORMATION_H_
#define MYSTERIO_OSBORN_MISSION_FORMATION_H_
#include <vector>
#include "../../src/utils/Coordinate.h"

class Formation {
public:
    Formation();
    virtual ~Formation();
private:
    int numberOfUAVs;
    std::vector<Coordinate> positions;
};

#endif
