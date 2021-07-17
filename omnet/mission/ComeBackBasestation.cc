#include "ComeBackBasestation.h"

ComeBackBasestation::ComeBackBasestation() { base = Coordinate(200,200,200); }

ComeBackBasestation::~ComeBackBasestation() { }

bool ComeBackBasestation::isComplete(){ } //Passar coordenate

void ComeBackBasestation::assignTask(){}

void ComeBackBasestation::assignTask(int idUAV, Coordinate target){
    this->base = target;
    this->idUAV = idUAV;
}
