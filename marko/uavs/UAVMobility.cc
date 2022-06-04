#include "../uavs/UAVMobility.h"
#include <iostream>
#include <queue>

#include "../communication/UAVMysCommunication.h"
#include "../../src/mission/DependentTask.h"
#include "../sheep/SheepMobility.h"
#include "UAVMessage_m.h"

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
std::vector<Task> tasksVector[NUMUAVS];
std::queue<TaskMessage> msgs;
UAVMysCommunication uavs[NUMUAVS];

int waypoints[NUMUAVS];
int lowbattery[NUMUAVS];
using namespace power;

Define_Module(UAVMobility);

UAVMobility::UAVMobility(){ nextMoveIsWait = false; }

void UAVMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);
    uav.setID(getParentModule()->getIndex());
    if(uav.getID() == 0){
        ativo[uav.getID()] = true;
        funcao = 3;
    }else{
        ativo[uav.getID()] = false;
        funcao = 2;
    }

    for (int i = 0; i < NUMUAVS; i++) {
        itera[i] = -1;
        waypoints[i] = 0;
    }

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        velocidade[uav.getID()] = par("speed").operator double();
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
    }
    this->rescueDataAndStoreVariables();
}

void UAVMobility::setTargetPosition() {

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    }else if(funcao == 2 && !ativo[uav.getID()]){
        simtime_t waitTime = waitTimeParameter->doubleValue()+3;
        nextChange = simTime() + waitTime;
        nextMoveIsWait = true;

//        Coord inicialPosition(500, 500, 500);
        targetPosition = lastPosition;
    }else if(funcao == 0 && !ativo[uav.getID()]) {

        Coord inicialPosition(10, 0, 10);

        if(targetPosition != inicialPosition){
            targetPosition = inicialPosition;
            double speed = speedParameter->doubleValue();
            double distance = lastPosition.distance(targetPosition);
            simtime_t travelTime = distance / speed;
            nextChange = simTime() + travelTime;
            nextMoveIsWait = hasWaitTime;
        }else{
            simtime_t waitTime = waitTimeParameter->doubleValue()+3;
            nextChange = simTime() + waitTime;
            nextMoveIsWait = false;
        }

    } else {
        if(tasksVector[uav.getID()].size() != itera[uav.getID()] && tasksVector[uav.getID()].size() > 0){ //if there are tasks not performed
            int task = itera[uav.getID()];
            //finalizando
            if(tasksVector[uav.getID()][task].getStatus() == 2){
                tasksVector[uav.getID()][task].setComplete();

                //enviando mensagem de finalizada
                NetworkConfigurations ntc;
                ntc.setIdSocket(uavs[uav.getID()].getSocketCode());
                this->uav.setNetworkConfigurations(ntc);
                UAVMysCommunication u;
                u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
                u.setSelfID(this->uav.getID());
                TaskMessage msg;
                msg.setCode(273);
                msg.setSource(u.getSelfID());
                msg.setTask(tasksVector[uav.getID()][task]);
                u.dispatchTaskMessage(msg);

                //next task
                itera[uav.getID()]++;

            }else{
                executeTask(task);
            }
        }else{
            UAVMysCommunication u;
            u.setSocketCode(this->uav.getNetworkConfigurations().getIdSocket());
            u.setSelfID(this->uav.getID());
            Message msg;
            msg.setCode(123);
            char m[] = "No scheduled tasks!";
            msg.setMsg(m);
            msg.setSource(u.getSelfID());
            u.dispatchMessage(msg);
            if(myStage++ == 0){
                Coord p(uniform(10, 25), uniform(10, 25), uniform(50, 80));
                targetPosition = p;
            }else{
                targetPosition = getRandomPosition();
            }

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
    this->rescueDataAndStoreVariables();
}

void UAVMobility::move() {
    //Drenando bateria e repassando tarefa
    if(lowbattery[uav.getID()] == 1){
        cout << "Drenou" << endl;
        cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
        SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
        energySto->consumir();
        lowbattery[uav.getID()] = 2;

        TaskMessage msg;
        msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
        msg.setSource(uav.getID());
        tasksVector[uav.getID()][itera[uav.getID()]].setWaypoints(waypoints[uav.getID()]);
        cout << "MEU WAYPOINT: " << tasksVector[uav.getID()][itera[uav.getID()]].getWaypoints() << endl;
        msg.setTask(tasksVector[uav.getID()][itera[uav.getID()]]);
        //targetPosition = getPosit(10, 5, 10);

        //aqui
        //msg.setCoord(this->castCoordToCoordinate(position[uav.getID()]));
        if(tasksVector[uav.getID()][itera[uav.getID()]].getType() == Task::FLY_AROUND_SQUARE){
            waypoints[uav.getID()]--;
            msg.setCoord(castCoordToCoordinate(flyAroundSquare(itera[uav.getID()])));
        }else{
            msg.setCoord(tasksVector[uav.getID()][itera[uav.getID()]].getTarget());
        }
        uavs[uav.getID()].dispatchTaskMessage(msg);
    }
    if(bateria[uav.getID()] < 0.005 && ativo[uav.getID()]){
        ativo[uav.getID()] = false;
    }
    if(continuoustask){
        analisarDistanciaOvelha();
    }
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
    this->rescueDataAndStoreVariables();

}

double UAVMobility::getMaxSpeed() const {
    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}

J UAVMobility::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getResidualEnergyCapacity();
}

Coord UAVMobility::pegarPosicaoOvelha(){
    cModule *a = getParentModule()->getParentModule()->getSubmodule("sheep")->getSubmodule("mobility", 0);
    SheepMobility *smob = check_and_cast<SheepMobility*>(a);

    return smob->posicaoAtual();
}

void UAVMobility::analisarDistanciaOvelha(){
    Coord sheep = pegarPosicaoOvelha();
    if(sheep.distance(lastPosition) < 300){
        cout << "UAV perto da ovelha!" << endl;
        continuoustask = false;
        waypoints[uav.getID()] = 2;
    }
}

void UAVMobility::rescueDataAndStoreVariables(){
    position[uav.getID()] = lastPosition;
    velocidade[uav.getID()] = speedParameter->doubleValue();
    bateria[uav.getID()] = std::stof(pegarBateria(uav.getID()).str());
    tempoVoo[uav.getID()] = simTime().dbl();
}

Coord UAVMobility::findSheep(int j){
    Coord c;
    analisarDistanciaOvelha();

    //checa se a ovelha está no raio de alcance
    //se sim, se comunica com ela
    //se não, o drone procura

    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        waypoints[uav.getID()]++;
        continuoustask = true;
    }else if(waypoints[uav.getID()] == 1){
        c = getRandomPosition();
        c.setZ(200);
    }else if(waypoints[uav.getID()] == 2){
        c = pegarPosicaoOvelha();
        c.setZ(200);
        waypoints[uav.getID()] = 0;
        tasksVector[uav.getID()][j].setStatus(2);

        //Parando verificação/busca pela ovelha
        continuoustask = false;

        TaskMessage tm;
        tm.setSource(this->uav.getID());
        tm.setDestination(-5);
        //preparando comunicação com ovelha
        msgs.push(tm);

        TaskMessage msg;
        Task t;
        t.setId(1);
        msg.setMsg("OLHA A MENSAGEM!");
        msg.setCode(Message::SUBORDINATE_SUBTASK);
        msg.setSource(uav.getID());
        msg.setDestination(1);
        msg.setTask(t);
        Coord d = c;
        if(d.getX() > 50 && d.getY() > 50){
            d.setX(d.getX()-50);
            d.setY(d.getY()-50);
        }
        msg.setCoord(this->castCoordToCoordinate(d));
        uavs[uav.getID()].dispatchTaskMessage(msg);

        msg.setDestination(2);
        t.setId(2);
        msg.setTask(t);
        d.setX(d.getX()+100);
        msg.setCoord(this->castCoordToCoordinate(d));
        uavs[uav.getID()].dispatchTaskMessage(msg);

        msg.setDestination(3);
        t.setId(3);
        msg.setTask(t);
        d.setY(d.getY()+100);
        msg.setCoord(this->castCoordToCoordinate(d));
        uavs[uav.getID()].dispatchTaskMessage(msg);

        msg.setDestination(4);
        t.setId(4);
        msg.setTask(t);
        d.setX(d.getX()-100);
        msg.setCoord(this->castCoordToCoordinate(d));
        uavs[uav.getID()].dispatchTaskMessage(msg);
    }

    return c;
}

Coord UAVMobility::surroundSheep(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        waypoints[uav.getID()]++;
    }else if(waypoints[uav.getID()] == 1){
        tasksVector[uav.getID()][j].setStatus(2);
        ativo[uav.getID()] = false;
        waypoints[uav.getID()] = 0;
        //nextMoveIsWait = true;
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
    }
    return c;
}

Coord UAVMobility::flyAround(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 1){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()-50);
    }else if(waypoints[uav.getID()] == 2){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+50);
        c.setY(c.getY()+50);
        if(uav.getID() == 0){
            cModule *a = getParentModule()->getParentModule()->getSubmodule("UAV", uav.getID())->getSubmodule("energyStorage", 0);
            SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
            energySto->consumir();
            TaskMessage msg;
            msg.setCode(Message::TASK_EMERGENCY_BATTERY_LOW);
            msg.setSource(uav.getID());
            tasksVector[uav.getID()][itera[uav.getID()]].setWaypoints(waypoints[uav.getID()]+1);
            msg.setTask(tasksVector[uav.getID()][itera[uav.getID()]]);
            //targetPosition = getPosit(10, 5, 10);
            //cout << "GASTOU!" << endl;
            msg.setCoord(this->castCoordToCoordinate(c));
            uavs[uav.getID()].dispatchTaskMessage(msg);
        }
    }else if(waypoints[uav.getID()] == 3){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-50);
        c.setY(c.getY()+50);
    }else if(waypoints[uav.getID()] == 5){
        //Finalizando task
        tasksVector[uav.getID()][j].setStatus(2);
        if(uav.getID() == 1){
            ativo[uav.getID()] = false;
        }

        //Próxima coordenada
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());

    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 5) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

Coord UAVMobility::flyAroundSquare(int j){
    Coord c;
    if(waypoints[uav.getID()] == 0 || waypoints[uav.getID()] == 4){
        //Finalizando task
        if(waypoints[uav.getID()] == 4){
            tasksVector[uav.getID()][j].setStatus(2);
            if(uav.getID() == 1){
                ativo[uav.getID()] = false;
            }
        }
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 1){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()-400);
    }else if(waypoints[uav.getID()] == 2){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()+400);
        c.setY(c.getY()+400);
    }else if(waypoints[uav.getID()] == 3){
        c = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        c.setX(c.getX()-400);
        c.setY(c.getY()+400);
    }
    waypoints[uav.getID()] = (waypoints[uav.getID()] < 4) ? waypoints[uav.getID()]+1 : 0;
    return c;
}

void UAVMobility::executeTask(int j){
    if(tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND){
        targetPosition = flyAround(j);
    }else if (tasksVector[uav.getID()][j].getType() == Task::FLY_AROUND_SQUARE){
        targetPosition = flyAroundSquare(j);
    }else if(tasksVector[uav.getID()][j].getType() == Task::FIND_SHEEP){
        targetPosition = findSheep(j);
    }else if(tasksVector[uav.getID()][j].getType() == Task::SURROUND_SHEEP){
        targetPosition = surroundSheep(j);
    }else{
        targetPosition = this->castCoordinateToCoord(tasksVector[uav.getID()][j].getTarget());
        itera[uav.getID()]++;
    }
}
