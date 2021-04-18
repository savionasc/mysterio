#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_

#include <stdio.h>

// deve ser implementado em todos os objetos comunicaveis (Componentes ou UAVs)
//This interface is importante to create Communicables classes
class Communicable { //Abstract Class?
public:
    //definir o tipo message
    virtual void onMessageReceive(int msg){
        printf("Communicable %d\n", msg);
    }
};

#endif
