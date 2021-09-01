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
    selfID = getParentModule()->getIndex();
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        pontos[NUMUAVS] = 0;
    }

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade[selfID] = par("speed").operator double();
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
        if(base[selfID].size() != itera[selfID] && base[selfID].size() > 0){
            cout << "ATRIBUIU!!!" << endl;
            executeTask(base[selfID].size()-1);
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
    position[selfID] = lastPosition;
    velocidade[selfID] = speedParameter->doubleValue();
    bateria[selfID] = std::stof(pegarBateria(selfID).str());
    tempoVoo[selfID] = simTime().dbl();
}

void UAVMobility::executeTask(int j){
    if(base[selfID][j]->type == FLY_AROUND){
        Coord c;
        if(pontos[selfID] == 0 || pontos[selfID] == 4){
            c = this->CoordinateToCoord(base[selfID][j]->target);
            c.setX(c.getX()-50);
            c.setY(c.getY()-50);
        }else if(pontos[selfID] == 1){
            c = this->CoordinateToCoord(base[selfID][j]->target);
            c.setX(c.getX()+50);
            c.setY(c.getY()-50);
        }else if(pontos[selfID] == 2){
            c = this->CoordinateToCoord(base[selfID][j]->target);
            c.setX(c.getX()+50);
            c.setY(c.getY()+50);
        }else if(pontos[selfID] == 3){
            c = this->CoordinateToCoord(base[selfID][j]->target);
            c.setX(c.getX()-50);
            c.setY(c.getY()+50);
        }else if(pontos[selfID] == 5){
            c = this->CoordinateToCoord(base[selfID][j]->target);
            itera[selfID]++;
        }
        targetPosition = c;
        pontos[selfID] = (pontos[selfID] < 5) ? pontos[selfID]+1 : 0;
    }else{
        targetPosition = this->CoordinateToCoord(base[selfID][j]->target);
        itera[selfID]++;
    }
}

void UAVMobility::stop(){ //Finaliza a atividade no Omnet
    nextMoveIsWait = true;
    nextChange = simTime() + 0.1;
    cout << "step" << endl;
    pular++;
}
