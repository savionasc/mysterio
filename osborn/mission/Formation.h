#ifndef MYSTERIO_OSBORN_MISSION_FORMATION_H_
#define MYSTERIO_OSBORN_MISSION_FORMATION_H_
#include <vector>
#include "../../src/utils/Coordinate.h"

class Formation {
public:
    Formation();
    virtual ~Formation();

    int getNumberOfUAVs();
    void setNumberOfUAVs(int numberOfUAVs);
    void addPosition(Coordinate position);
    Coordinate getPosition(int i);
    std::vector<Coordinate> getAllPositions();

private:
    int numberOfUAVs;
    std::vector<Coordinate> positions;
};

#endif
