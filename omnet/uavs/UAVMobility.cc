#include "../uavs/UAVMobility.h"
#include "../mission/GoToTask.h"
#include <iostream>
#include "../scenarios/Example1Communication.h"
#include "../communication/UAVCommunicationSocket.h"
#include "../../src/utils/Codes.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

Coord position[NUMUAVS];
double velocidade[NUMUAVS];
float bateria[NUMUAVS];
double tempoVoo[NUMUAVS];
bool ativo[NUMUAVS];// = {true, true};
int itera[NUMUAVS];
GoToTask minhasTarefas[NUMUAVS][5]; //Task
std::vector<Task*> base; //Task
UAVCommunicationSocket uavs[NUMUAVS];
int stg = 0;
//Matriz de tarefas...
//[uav][tarefa]

//1 - Tarefa: decolar (idUAV)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
int UAVLeader = -1;
int UAVDestino = -1;

using namespace power;

Define_Module(UAVMobility);

    //Para contar o tempo, talvez adicionar no subscribe ou aqui no próprio uav
    //clock_t t;
    //t = clock();
    //t = clock() - t;
    //cout << ((double)t)/((CLOCKS_PER_SEC/1000)) << endl;

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
    //std::vector<BaseClass*> base;
    //base.push_back(new FirstDerivedClass());
    //base.push_back(new SecondDerivedClass());
    cout << "itera: " << itera[selfID] << endl;
    if(itera[0] > -1 && base[0]->type == 10){
        cout << "Deu certo!!!!" << endl;
        Task* taskPtr = base[0]; //&gotoc;
        GoToTask* gotoPtr = dynamic_cast<GoToTask*>(taskPtr);

        cout << "Type:" << base[0]->type << endl;
        cout << "started:" << gotoPtr->started << endl;
        cout << "initial:" << gotoPtr->initialPosition.getX() << gotoPtr->initialPosition.getY() << gotoPtr->initialPosition.getZ() << endl;
    }
    //Verificando se está parado
    Coordinate currentPosition(lastPosition.x,lastPosition.y,lastPosition.z);
    if(minhasTarefas[selfID][itera[selfID]].isComplete(currentPosition)){
        itera[selfID]++;
        //nextMoveIsWait = true;
    }

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
        cout << "Caiu no nextMoveIsWait" << endl;
    } else {
        if(minhasTarefas[selfID][itera[selfID]].started){
            cout << "Caiu no minhasTarefas[selfID][0].started" << endl;
            targetPosition = this->CoordinateToCoord(minhasTarefas[selfID][itera[selfID]].target);
            Coord x = this->CoordinateToCoord(minhasTarefas[selfID][itera[selfID]].target);
            cout << x << endl;
        }else if(stg <= NUMUAVS){
            Coord ini(220, 220, 220);
            targetPosition = ini;
            cout << "Caiu no ini" << endl;
            stg++;
        }else{
            cout << "Caiu no getRandomPosition()" << endl;
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
    if(ativo[selfID]){
        if(!minhasTarefas[selfID][itera[selfID]].started){
            minhasTarefas[selfID][itera[selfID]].started = true;
            this->setTargetPosition();
            //targetPosition = this->CoordinateToCoord(minhasTarefas[selfID][0].target); //colocar um i para tarefa atual
        }
        LineSegmentsMobilityBase::move();
        raiseErrorIfOutside();
        this->rescueData();
    }
}

//CheckTasks

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility::pegarBateria(int idUAV){ //Retirar daqui e passar para o Mobility
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
