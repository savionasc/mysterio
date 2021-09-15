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

class Task { //Goto, ComeBackBaseStation

public:
    Task(){}
    Task(UAV uav, Coordinate targetPosition) { // adicionar type no construtor
        this->uav = uav;
        this->target = targetPosition;
    }
    virtual ~Task(){}

    virtual bool isComplete(){
        return (this->status == COMPLETED) ? true : false; //complete
    }

    void setStatus(int status){
        this->status = status;
    }

    void setComplete(){
        this->status = COMPLETED;
    }

    int getID() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    int getRepeat() const {
        return repeat;
    }

    void setRepeat(int repeat = 0) {
        this->repeat = repeat;
    }

    int getStatus() const {
        return status;
    }

    const Coordinate& getTarget() const {
        return target;
    }

    void setTarget(const Coordinate& target) {
        this->target = target;
    }

    int getType() const {
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
};

#endif
