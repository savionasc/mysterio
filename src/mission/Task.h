#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"
#include "../utils/UAV.h"
#include "Command.h"
//States
#define STARTED 1
#define COMPLETED 2
//Types
#define GOTO 80
#define COMEBACK_BASESTATION 81
#define FLY_AROUND 82

class Task { //Goto, ComeBackBaseStation

public:
    Task(){}
    Task(UAV uav, Coordinate targetPosition) {
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

    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
    //int idUAV;
    int id;
    UAV uav;
    int type;
    //Command cmd; //Criar uma lista/fila de comandos..
    int status = 0; //status{started, completed}
    Coordinate target;
    int repeat = 0; //TaskManager
};

#endif
