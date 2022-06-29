#include "Formation.h"

Formation::Formation() { }
Formation::Formation(int numberOfUAVs) {
    this->numberOfUAVs = numberOfUAVs;
}

Formation::Formation(int numberOfUAVs, int leader) {
    this->numberOfUAVs = numberOfUAVs;
    this->leader = leader;
}

Formation::~Formation() { }

int Formation::getLeader() {
    return leader;
}

void Formation::setLeader(int leader) {
    this->leader = leader;
}

int Formation::getNumberOfUAVs() {
    return numberOfUAVs;
}

void Formation::setNumberOfUAVs(int numberOfUAVs) {
    this->numberOfUAVs = numberOfUAVs;
}

void Formation::addPosition(Coordinate position){
    this->positions.push_back(position);
}

Coordinate Formation::getPosition(int i){
    return this->positions[i];
}

std::vector<Coordinate> Formation::getAllPositions(){
    return this->positions;
}
