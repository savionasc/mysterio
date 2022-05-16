#include "ComeBackBasestation.h"

ComeBackBasestation::ComeBackBasestation() { base = Coordinate(200,200,200); }

ComeBackBasestation::~ComeBackBasestation() { }

//Passar coordenate
bool ComeBackBasestation::isComplete(){ }

void ComeBackBasestation::assignTask(){}

void ComeBackBasestation::assignTask(UAV uav, Coordinate target){
    this->base = target;
    this->setUAV(uav);
}
