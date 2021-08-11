#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"
#include "Command.h"

class Task { //Goto, ComeBackBaseStation

public:
    Task(){}
    Task(int idUAV, Coordinate targetPosition) {
        this->idUAV = idUAV;
        this->target = targetPosition;
        //this->assignTask(idUAV, targetPosition);
    }
    virtual ~Task(){}

    virtual bool isComplete(){
        return this->finished; //complete
    }

    //Task(Command command(int *args[]))
    //TaskManager(Task, idUAV)
    virtual void assignTask(int idUAV, Command command, int *args[]){ //Drone, comando, parametros do comando
        this->idUAV = idUAV;
        this->cmd = command;
    }

    //TaskManager
    virtual void markAsComplete(Coordinate c){//Task
        //Task.uav.get
        if(c.getX() == target.getX() && c.getY() == target.getY() && c.getZ() == target.getZ())
            this->finished = true;
    }

    //Criar uma setComplete



    //int Priority?
    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
    int idUAV; //UAV
    int type;
    Command cmd; //Remover
    bool started = false; //status
    bool finished = false;
    Coordinate target;
    int repeat = 0; //TaskManager
};

#endif
