#include "TaskAssistant.h"
#include <math.h>
#include <stdio.h>

using namespace std;

TaskAssistant::TaskAssistant() {}

TaskAssistant::~TaskAssistant() {}

std::vector<Coordinate> TaskAssistant::splitCoordinate(Coordinate coord, int slice){

    /*int size = qtyDivisions(coord, 50);
    Coordinate coords[size];
    coordinatesToFollow(coord, coords, size);
    for (int var = 0; var < size; ++var) {
        v[uav]->push_back(coord[var]);
        cout << "PRINT " << var << endl;
        cout << "x: " << coords[var].getX() << " y: " << coords[var].getY() << " z: " << coords[var].getZ() << endl;
    }*/

    vector<Coordinate> v;
    int size = qtyDivisions(coord, slice);
    cout << "size: " << size << endl;
    coordinatesToFollowVector(coord, &v, size);

    return v;
}

int TaskAssistant::qtyDivisions(Coordinate coord, int divider){
    int x, y, z, quotient = 0;

    cout << "qty x: " << coord.getX() << " y: " << coord.getY() << " z: " << coord.getZ() << endl;

    //quotient calculation
    x = coord.getX()/divider;
    y = coord.getY()/divider;
    z = coord.getZ()/divider;

    cout << "divider x: " << x << " y: " << y << " z: " << z << endl;

    //returns the largest of the 3 quotient divisions
    if(x >= y && x >= z)
        return x;
    else if(y >= x && y >= z)
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
        Coordinate c(x, y, z);
        coords[i] = c;
    }
}

void TaskAssistant::coordinatesToFollowVector(Coordinate coord, std::vector<Coordinate> *v, int length){
    for (int i = 0; i < length; i++) {
        float x, y, z;
        x = (floor((coord.getX())) / length ) * (i+1);
        y = floor( (coord.getY() / length) * (i+1) );
        z = floor( (coord.getZ() / length) * (i+1) );
        Coordinate c(x, y, z);
        v->push_back(c);
    }
}
