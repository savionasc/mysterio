#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include "Command.h"
#include "Priority.h"

class Task {

public:
    Task(){}
    Task(UAV uav, Coordinate targetPosition) {
        // adicionar type no construtor
        this->uav = uav;
        this->target = targetPosition;
    }
    Task(UAV uav, int type, Coordinate targetPosition) {
        // adicionar type no construtor
        this->uav = uav;
        this->type = type;
        this->target = targetPosition;
    }
    virtual ~Task(){}

    enum statestasks{
        STARTED = 1,
        COMPLETED
    };

    enum typetasks{
        GOTO = 80,
        COMEBACK_BASESTATION,
        FLY_AROUND,
        FLY_AROUND_SQUARE,
        FIND_SHEEP,
        SURROUND_SHEEP,
        SPLITTEDGOTO
    };

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

    int getPriority() {
        return priority;
    }

    void setPriority(int priority = 0) {
        this->priority = priority;
    }

    int getAssincrono() const {
        return assincrono;
    }

    void setAssincrono(int assincrono = 0) {
        this->assincrono = assincrono;
    }

private:
    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
    int id;
    UAV uav;
    int type;
    //Criar uma lista/fila de comandos..
    Command cmd;
    //status{started, completed}
    int status = 0;
    Coordinate target;
    //TaskManager
    int repeat = 0;
    int waypoints;
    int priority = 0;
    int assincrono = 0;
    Priority pri;
};

#endif
