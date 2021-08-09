#include "../uavs/UAVMobility.h"
<<<<<<< HEAD
#include "../mission/GoTo.h"
=======
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c
#include <iostream>
#include "../scenarios/Example1Communication.h"
#include "../communication/UAVCommunicationSocket.h"
#include "../../src/utils/Codes.h"
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
//GoTo minhasTarefas[NUMUAVS][5]; //Task
std::vector<Task*> base[NUMUAVS]; //Task
UAVCommunicationSocket uavs[NUMUAVS];

//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
int UAVLeader = -1;
int UAVDestino = -1;

using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    selfID = getParentModule()->getIndex();
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
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

    cout << "itera: " << itera[selfID] << " size: " << base[selfID].size() << endl;
<<<<<<< HEAD
    //Verificar se é uma atividade disso, ou atividade daquilo pelo type
    //if(itera[0] > -1 && base[0][0]->type == 10){

    //Checando se a atividade está completa pra passar para outra tarefa
    //if(gotoPtr->isComplete(currentPosition)){
        //itera[selfID]++;
        //nextMoveIsWait = true;
    //}
=======
    if(itera[selfID] > -1 && base[selfID][0]->type == 10){
        cout << "Deu certo!!!!" << endl;
        Task* taskPtr = base[selfID][0]; //&gotoc;
        GoTo* gotoPtr = dynamic_cast<GoTo*>(taskPtr);

        cout << "Type:" << base[selfID][0]->type << endl;
        cout << "started:" << gotoPtr->started << endl;
        cout << "initial:" << gotoPtr->initialPosition.getX() << gotoPtr->initialPosition.getY() << gotoPtr->initialPosition.getZ() << endl;
    }else if(itera[selfID] > -1 && base[selfID][0]->type == 11){
        cout << "Deu certo!!!!" << endl;
        Task* taskPtr = base[selfID][0]; //&gotoc;
        GoTo* gotoPtr = dynamic_cast<GoTo*>(taskPtr);

        cout << "Type:" << base[selfID][0]->type << endl;
        cout << "started:" << gotoPtr->started << endl;
        cout << "initial:" << gotoPtr->initialPosition.getX() << gotoPtr->initialPosition.getY() << gotoPtr->initialPosition.getZ() << endl;
    }
    if(itera[selfID] > -1){
        cout << "entrou!" << endl;
        //Verificando se está parado
        Coordinate currentPosition(lastPosition.x,lastPosition.y,lastPosition.z);
        //if(minhasTarefas[selfID][itera[selfID]].isComplete(currentPosition)){
        Task* taskPtr = base[selfID][itera[selfID]]; //&gotoc;
        GoTo* gotoPtr = dynamic_cast<GoTo*>(taskPtr);
        if(gotoPtr->isComplete(currentPosition)){
            //itera[selfID]++;
            //cout << "itera: " << itera[selfID] << " size: " << base[selfID].size() << endl;

            //nextMoveIsWait = true;
        }
    }
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
<<<<<<< HEAD
        if(base[selfID].size() != itera[selfID] && base[selfID].size() > 0){
            cout << "ATRIBUIU!!!" << endl;
            //Coord ini(100.0, 100.0, 100.0);
            //targetPosition = ini;
            targetPosition = this->CoordinateToCoord(base[selfID][0]->target);
            itera[selfID]++;
=======
        cout << "Else" << endl;
        //if(minhasTarefas[selfID][itera[selfID]].started){
        if(itera[selfID] > -1 && base[selfID][itera[selfID]]->started){
            cout << "Caiu no minhasTarefas[selfID][0].started" << endl;
            Task* taskPtr = base[selfID][itera[selfID]]; //&gotoc;
            GoTo* gotoPtr = dynamic_cast<GoTo*>(taskPtr);
            targetPosition = this->CoordinateToCoord(gotoPtr->target);
            Coord x = this->CoordinateToCoord(gotoPtr->target);
            cout << x << endl;
        }else if(stg <= NUMUAVS){
            Coord ini(220, 220, 220);
            targetPosition = ini;
            cout << "Caiu no ini" << endl;
            stg++;
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c
        }else{
            targetPosition = getRandomPosition();
        }
        cout << "target-" << selfID <<": " << targetPosition << endl;
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->rescueData();
}

void UAVMobility::move() {
    //if(ativo[selfID]){
        //if(!base[selfID][itera[selfID]]->started){
            //base[selfID][itera[selfID]]->started = true;
            //this->setTargetPosition();
            //targetPosition = this->CoordinateToCoord(minhasTarefas[selfID][0].target); //colocar um i para tarefa atual
        //}
        LineSegmentsMobilityBase::move();
        raiseErrorIfOutside();
        this->rescueData();
    //}
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
