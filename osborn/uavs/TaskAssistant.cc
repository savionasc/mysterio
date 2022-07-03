#include "TaskAssistant.h"
#include <math.h>
#include <stdio.h>

using namespace std;

TaskAssistant::TaskAssistant() {}

TaskAssistant::~TaskAssistant() {}

std::vector<Coordinate> TaskAssistant::splitCoordinate(Coordinate coord, int slice){
    vector<Coordinate> v;
    int size = qtyDivisions(coord, slice);
    coordinatesToFollowVector(coord, &v, size);

    return v;
}

std::vector<Coordinate> TaskAssistant::splitCoordinateFormation(Coordinate coord, Coordinate target, int slice){
    vector<Coordinate> v;
    Coordinate diff = diference(target, coord);
    int size = qtyDivisions(diff, slice);
    cout << "MEU SIZE: " << size << endl;
    coordinatesToFollowVectorFormation(coord, diff, &v, size);

    return v;
}

int TaskAssistant::qtyDivisions(Coordinate coord, int divider){
    int x, y, z, quotient = 0;

    //quotient calculation
    x = ((coord.getX() > 0) ? coord.getX() : (coord.getX() * (-1)))/divider;
    y = ((coord.getY() > 0) ? coord.getY() : (coord.getY() * (-1)))/divider;
    z = ((coord.getZ() > 0) ? coord.getZ() : (coord.getZ() * (-1)))/divider;

    //returns the largest of the 3 quotient divisions
    if(x >= y && x >= z)
        return x;
    else if(y >= x && y >= z)
        return y;
    else
        /*if(z <= 0){

        }else{*/
            return z;
       // }
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

void TaskAssistant::coordinatesToFollowVectorFormation(Coordinate coord, Coordinate diff, std::vector<Coordinate> *v, int length){
    for (int i = 0; i < length; i++) {
        float x, y, z;
        x = (floor((diff.getX())) / length ) * (i+1);
        y = floor( (diff.getY() / length) * (i+1) );
        z = floor( (diff.getZ() / length) * (i+1) );
        Coordinate c(coord.getX()+x, coord.getY()+y, coord.getZ()+z);
        v->push_back(c);
    }
}

Coordinate TaskAssistant::diference(Coordinate coord1, Coordinate coord2){
    return Coordinate(
            coord1.getX()-coord2.getX(),
            coord1.getY()-coord2.getY(),
            coord1.getZ()-coord2.getZ());
}
