#ifndef MYSTERIO_SRC_MISSION_TASK_H_
#define MYSTERIO_SRC_MISSION_TASK_H_
#include "../utils/Coordinate.h"

class Task {
public:
    //Função de Avaliação
    virtual bool isComplete(){ } //For GoTo Task, Current Coordinate is a parameter necessary to check this
    virtual void setTask(){ }

    char objetivo[150]; //Definir um tipo Objetivo
    Coordinate target;
    Coordinate initialPosition;

    //Talvez calculemos o progresso num futuro próximo...
    //Se for da missão é saber quantas tarefas foram cumpridas e quantas não foram
    //Se for da tarefa é saber quanto falta para chegar no destino
};

#endif
