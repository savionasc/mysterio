#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"
#include "Command.h"

class Task { //Goto, ComeBackBaseStation

public:
    //Função de Avaliação
    Task(){}
    Task(int idUAV, Coordinate targetPosition) {
        this->idUAV = idUAV;
        this->target = targetPosition;
        //this->assignTask(idUAV, targetPosition);
    }
    virtual ~Task(){}
    virtual bool isComplete(){ } //For GoTo Task, Current Coordinate is a parameter necessary to check this
    virtual void assignTask(int idUAV, Command command, int *args[]){ //Drone, comando, parametros do comando
        this->idUAV = idUAV;
        this->cmd = command;
    }


    //int Priority?
    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
    int idUAV;
    int type;
    Command cmd;
    bool started;
    Coordinate target;
    //Colocar o target de novo?
};

#endif
