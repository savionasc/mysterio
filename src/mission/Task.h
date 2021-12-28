#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include "Command.h"
//States
#define STARTED 1
#define COMPLETED 2 //Colocar perto de completar...
//Types
#define GOTO 80
#define COMEBACK_BASESTATION 81
#define FLY_AROUND 82
#define FLY_AROUND_SQUARE 83
#define FIND_SHEEP 84

class Task {

public:
    Task(){}
    Task(UAV uav, Coordinate targetPosition) { // adicionar type no construtor
        this->uav = uav;
        this->target = targetPosition;
    }
    Task(UAV uav, int type, Coordinate targetPosition) { // adicionar type no construtor
        this->uav = uav;
        this->type = type;
        this->target = targetPosition;
    }
    virtual ~Task(){}

    int getID() {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    virtual bool isComplete(){
        return (this->status == COMPLETED) ? true : false; //complete
    }

    void setComplete(){
        this->status = COMPLETED;
    }

    int getRepeat() {
        return repeat;
    }

    void setRepeat(int repeat = 0) {
        this->repeat = repeat;
    }

    int getStatus() {
        return status;
    }

    void setStatus(int status){
        this->status = status;
    }

    Coordinate getTarget(){
        return target;
    }

    void setTarget(Coordinate target) {
        this->target = target;
    }

    int getType() {
        return type;
    }

    void setType(int type) {
        this->type = type;
    }

    UAV getUAV() {
        return uav;
    }

    void setUAV(UAV uav) {
        this->uav = uav;
    }

    Command getCmd() {
        return cmd;
    }

    void setCmd(Command cmd) {
        this->cmd = cmd;
    }

    virtual void executeTask(){

    }

    int getWaypoints() {
        return waypoints;
    }

    void setWaypoints(int waypoints) {
        this->waypoints = waypoints;
    }

private:
    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
    //int idUAV;
    int id;
    UAV uav;
    int type;
    Command cmd; //Criar uma lista/fila de comandos..
    int status = 0; //status{started, completed}
    Coordinate target;
    int repeat = 0; //TaskManager
    int waypoints;
};

#endif
