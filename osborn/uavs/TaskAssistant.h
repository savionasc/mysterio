#ifndef MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#define MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#include <vector>

#include "../../src/utils/Coordinate.h"

using namespace std;

class TaskAssistant {
public:
    TaskAssistant();
    virtual ~TaskAssistant();
    //Coordinate* splitCoordinate(Coordinate coord);
    std::vector<Coordinate> splitCoordinate(Coordinate coord);

private:
    int qtyDivisions(Coordinate c, int divider);
    void coordinatesToFollow(Coordinate coord, Coordinate *coords, int length);
    void coordinatesToFollowVector(Coordinate coord, std::vector<Coordinate> *v, int length);
};

#endif /* MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_ */
