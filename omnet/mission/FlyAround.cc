#include "FlyAround.h"

FlyAround::FlyAround() { }

FlyAround::FlyAround(int idUAV, Coordinate point, int laps) {
    this->idUAV = idUAV;
    this->point = point;
    this->laps = laps;
}

FlyAround::~FlyAround() { }

bool FlyAround::isComplete() {
    if(lap == this->laps)
        return true;
    return false;
}

//O drone vai interpretar o comando e pegar os args
//if command == "go"
//o drone vai interpretar que tem que ir pras coordenadas passadas em args...
void FlyAround::assignTask(int idUAV, Command command, int *args[]){ //Drone, comando, parametros do comando
    this->idUAV = idUAV;
    this->cmd = command;
}
