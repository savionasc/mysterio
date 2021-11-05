#include "UAVMobility2.h"
#include <iostream>

#include "../../communication/UAVMysCommunication.h"
#include "../../scenarios/Example1Communication.h"
#include "../../../src/mission/DependentTask.h"
#include "../../mission/MysTask.h"

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
extern bool ativo[NUMUAVS];
extern int itera[NUMUAVS];
extern std::vector<Task> base[NUMUAVS];
extern std::vector<MysTask> mysTasks[NUMUAVS];
extern UAVMysCommunication uavs[NUMUAVS];
extern int pular; //this variable forces terminate current "task" of uav
//1 - Tarefa: decolar (idUAV, altura)
//2 - Tarefa: goto (idUAV, positionTarget)
//3 - Tarefa: dar uma volta sobre (idUAV, positionTarget, distanciaSobre)
//4 - Tarefa: retornar a base (idUAV, basePosition)
//5 - Tarefa: pousar (idUAV, chao)
extern int UAVLeader;
extern int UAVDestino;
extern int waypoints[NUMUAVS];

using namespace power;

Define_Module(UAVMobility2);

UAVMobility2::UAVMobility2(){ nextMoveIsWait = false; }

void UAVMobility2::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());
    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        waypoints[NUMUAVS] = 0;
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

void UAVMobility2::setTargetPosition() {

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    } else {
        if(base[uav.getID()].size() != itera[uav.getID()] && base[uav.getID()].size() > 0){ //if there are tasks not performed
            int task = itera[uav.getID()];
            if(base[uav.getID()][task].getStatus() == 2){ //finalizando
                base[uav.getID()][task].setComplete();

                //enviando mensagem de finalizada
                this->uav.setIdSocket(uavs[uav.getID()].getSocketCode());
                UAVMysCommunication u;
                u.setSocketCode(this->uav.getIdSocket());
                u.setSelfID(this->uav.getID());
                TaskMessage msg;
                msg.setCode(273);
                msg.setSource(u.getSelfID());
                msg.setTask(base[uav.getID()][task]);
                u.dispatchTaskMessage(msg);

                //next task
                itera[uav.getID()]++;

            }else{
                executeTask(task);
            }
        }else{
            UAVMysCommunication u;
            u.setSocketCode(this->uav.getIdSocket());
            u.setSelfID(this->uav.getID());
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getSelfID());
            u.dispatchMessage(msg);
            targetPosition = getRandomPosition();
            if(u.getSelfID() == 1){
                u.disconnectBase();
            }
        }
        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);
        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;
    }
    this->rescueData();
}

void UAVMobility2::move() {
    if(pular == 1)
        this->stop();
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueData();
}

double UAVMobility2::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility2::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("host", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

void UAVMobility2::rescueData(){
    position[uav.getID()] = lastPosition;
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
}

Coord UAVMobility2::flyAround(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 1){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 2){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 3){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 5){
        //Finalizando task
        base[uav.getID()][j].setStatus(2);

        //Próxima coordenada
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());

    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 5) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

Coord UAVMobility2::flyAroundSquare(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        //Finalizando task
        if(waypoints[uav.getID()] == 4)
            base[uav.getID()][j].setStatus(2);
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 1){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 2){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(waypoints[uav.getID()] == 3){
        c = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 4) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

void UAVMobility2::executeTask(int j){
    if(base[uav.getID()][j].getType() == FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (base[uav.getID()][j].getType() == FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else{
        targetPosition = this->CoordinateToCoord(base[uav.getID()][j].getTarget());
        itera[uav.getID()]++;
    }
}

void UAVMobility2::stop(){ //Finaliza a atividade no Omnet
    nextMoveIsWait = true;
    nextChange = simTime() + 0.1;
    cout << "step" << endl;
    pular++;
}
