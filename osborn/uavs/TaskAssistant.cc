#include "TaskAssistant.h"
#include <math.h>
#include <stdio.h>
using namespace std;

TaskAssistant::TaskAssistant() {}

TaskAssistant::~TaskAssistant() {}

Coordinate* TaskAssistant::splitCoordinate(Coordinate coord){
    int size = qtyDivisions(coord, 50);
    Coordinate coords[size];
    coordinatesToFollow(coord, coords, size);
    for (int var = 0; var < size; ++var) {
        cout << "PRINT " << var << endl;
        cout << "x: " << coords[var].getX() << " y: " << coords[var].getY() << " z: " << coords[var].getZ() << endl;
    }
    return nullptr;
}

int TaskAssistant::qtyDivisions(Coordinate coord, int divider){
    int x, y, z, quotient = 0;

    //quotient calculation
    x = coord.getX()/divider;
    y = coord.getY()/divider;
    z = coord.getZ()/divider;

    //returns the largest of the 3 quotient divisions
    if(x > y && x > z)
        return x;
    else if(y > x && y > z)
        return y;
    else
        return z;
}

void TaskAssistant::coordinatesToFollow(Coordinate coord, Coordinate *coords, int length){
    for (int i = 0; i < length; i++) {
        float x, y, z;
        x = (floor((coord.getX())) / length ) * (i+1);
        y = floor( (coord.getY() / length) * (i+1) );
        z = floor( (coord.getZ() / length) * (i+1) );
        //printf("x: %f, y: %f, z: %f", x, coord.getY(), coord.getZ());
        Coordinate c(x, y, z);
        coords[i] = c;
    }
}
