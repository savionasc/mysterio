#ifndef MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#define MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#include <vector>

#include "../../src/utils/Coordinate.h"

using namespace std;

class TaskAssistant {
public:
    TaskAssistant();
    virtual ~TaskAssistant();
    std::vector<Coordinate> splitCoordinate(Coordinate coord, int slice);
    std::vector<Coordinate> splitCoordinateFormation(Coordinate coord, Coordinate target, int slice);

private:
    int qtyDivisions(Coordinate c, int divider);
    void coordinatesToFollow(Coordinate coord, Coordinate *coords, int length);
    void coordinatesToFollowVector(Coordinate coord, std::vector<Coordinate> *v, int length);
    void coordinatesToFollowVectorFormation(Coordinate coord, Coordinate diff, std::vector<Coordinate> *v, int length);
    Coordinate diference(Coordinate coord1, Coordinate coord2);
};

#endif
