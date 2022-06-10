#ifndef MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#define MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_
#include "../../src/utils/Coordinate.h"

class TaskAssistant {
public:
    TaskAssistant();
    virtual ~TaskAssistant();
    Coordinate* splitCoordinate(Coordinate coord); //splitCoordinate

private:
    int qtyDivisions(Coordinate c, int divider);
    void coordinatesToFollow(Coordinate coord, Coordinate *coords, int length);
};

#endif /* MYSTERIO_OSBORN_UAVS_TASKASSISTANT_H_ */
