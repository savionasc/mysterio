#ifndef MYSTERIO_OSBORN_MISSION_FORMATION_H_
#define MYSTERIO_OSBORN_MISSION_FORMATION_H_
#include <vector>
#include "../../src/utils/Coordinate.h"

class Formation {
public:
    Formation();
    Formation(int numberOfUAVs);
    Formation(int numberOfUAVs, int leader);
    virtual ~Formation();

    int getLeader();
    void setLeader(int leader);
    int getNumberOfUAVs();
    void setNumberOfUAVs(int numberOfUAVs);
    void addPosition(Coordinate position);
    Coordinate getPosition(int i);
    std::vector<Coordinate> getAllPositions();

private:
    int leader = -1;
    int numberOfUAVs;
    std::vector<Coordinate> positions;
};

#endif
