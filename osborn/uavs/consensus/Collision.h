#ifndef MYSTERIO_OSBORN_UAVS_CONSENSUS_COLLISION_H_
#define MYSTERIO_OSBORN_UAVS_CONSENSUS_COLLISION_H_

#include "../../../src/utils/UAV.h"

class Collision {
public:
    Collision();
    Collision(int uavCase, UAV uav);
    virtual ~Collision();

    enum CASES{
        CASE_UP_OR_DOWN = 1,
        CASE_UP = 2,
        CASE_DOWN = 3,
        CASE_NOT_UP_DOWN = 4
    };

    int getUAVCase();
    void setUAVCase(int uavCase);
    UAV getUAV();
    void setUAV(UAV uav);
    Coordinate getCoordinate();
    void setCoordinate(Coordinate coordinate);


    int UAVCase;
    UAV uav;
};

#endif
