#include "../uavs/UAVMobility.h"
#include <iostream>
#include "../scenarios/Example1Communication.h"
#include "../communication/UAVCommunicationSocket.h"
#include "../mission/GoTo.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

Coord position[NUMUAVS];
double velocidade[NUMUAVS];
float bateria[NUMUAVS];
double tempoVoo[NUMUAVS];
bool ativo[NUMUAVS];
int itera[NUMUAVS];
std::vector<Task*> base[NUMUAVS]; //Task
UAVCommunicationSocket uavs[NUMUAVS];
int pular = 0; //this variable forces terminate current "task" of uav
//std::vector<UAVCommunicationSocket> uavs;
//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
int UAVLeader = -1;
int UAVDestino = -1;
int pontos[NUMUAVS];

using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        pontos[NUMUAVS] = 0;
    }

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade[uav.getID()] = par("speed").operator double();
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
    this->rescueData();
}

void UAVMobility::setTargetPosition() {

    //cout << "itera: " << itera[selfID] << " size: " << base[selfID].size() << endl;

    //Checando se a atividade está completa pra passar para outra tarefa
    //if(gotoPtr->isComplete(currentPosition)){
        //itera[selfID]++;
        //nextMoveIsWait = true;
    //}

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        if(base[uav.getID()].size() != itera[uav.getID()] && base[uav.getID()].size() > 0){ //if there are tasks not performed
            executeTask(base[uav.getID()].size()-1);
        }else{
            targetPosition = getRandomPosition();
        }
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->rescueData();
}

void UAVMobility::move() {
    if(pular == 1)
        this->stop();
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueData();
}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("host", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

void UAVMobility::rescueData(){
    position[uav.getID()] = lastPosition;
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
}

void UAVMobility::executeTask(int j){
    if(base[uav.getID()][j]->type == FLY_AROUND){
        Coord c;
        if(pontos[uav.getID()] == 0 || pontos[uav.getID()] == 4){
            c = this->CoordinateToCoord(base[uav.getID()][j]->target);
            c.setX(c.getX()-50);
            c.setY(c.getY()-50);
        }else if(pontos[uav.getID()] == 1){
            c = this->CoordinateToCoord(base[uav.getID()][j]->target);
            c.setX(c.getX()+50);
            c.setY(c.getY()-50);
        }else if(pontos[uav.getID()] == 2){
            c = this->CoordinateToCoord(base[uav.getID()][j]->target);
            c.setX(c.getX()+50);
            c.setY(c.getY()+50);
        }else if(pontos[uav.getID()] == 3){
            c = this->CoordinateToCoord(base[uav.getID()][j]->target);
            c.setX(c.getX()-50);
            c.setY(c.getY()+50);
        }else if(pontos[uav.getID()] == 5){
            c = this->CoordinateToCoord(base[uav.getID()][j]->target);
            base[uav.getID()][j]->setComplete();
            itera[uav.getID()]++;
        }
        targetPosition = c;
        pontos[uav.getID()] = (pontos[uav.getID()] < 5) ? pontos[uav.getID()]+1 : 0;
    }else{
        targetPosition = this->CoordinateToCoord(base[uav.getID()][j]->target);
        itera[uav.getID()]++;
    }
}

void UAVMobility::stop(){ //Finaliza a atividade no Omnet
    nextMoveIsWait = true;
    nextChange = simTime() + 0.1;
    cout << "step" << endl;
    pular++;
}
