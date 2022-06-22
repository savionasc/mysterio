#ifndef MYSTERIO_OSBORN_UAVS_CONSENSUSALGORITHM_H_
#define MYSTERIO_OSBORN_UAVS_CONSENSUSALGORITHM_H_
#include <vector>
#include "../../../src/utils/UAV.h"
#include "inet/common/geometry/common/Coord.h"
#include "Collision.h"
//#include "../../src/utils/Coordinate.h"

class ConsensusAlgorithm {
public:
    ConsensusAlgorithm(Coordinate coord, std::vector<UAV> uavs);
    virtual ~ConsensusAlgorithm();

    int run();
    void makeDecision(Collision collision);
    Coordinate escapeCoordinate();
    int getNumberOfCollisions();
    std::vector<Collision> getCollisions();

private:
    inet::Coord castCoordinateToCoord(Coordinate co){
        inet::Coord coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    Coordinate castCoordToCoordinate(inet::Coord co){
        Coordinate coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    //Variables
    std::vector<UAV> uavs;
    std::vector<Collision> colisions;
    Coordinate coordinate;
    int numColisions = 0;
    int state = Collision::CASE_UP_OR_DOWN;

    //flag
    int runned = 0;

    //0 para run não executado
    //1 para run executado
    //2 para makeDecision com decisão curta
    //3 para makeDecision com decisão longa
};

#endif
