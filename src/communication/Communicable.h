#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_

#include <stdio.h>

// deve ser implementado em todos os objetos comunicaveis (componentes, uav, etc)
//This interface is importante to create Communicables classes
class Communicable { //Classe Abstrata
public:
    //Communicable();
    //virtual ~Communicable();
    //definir o tipo message
    virtual void onMessageReceive(int a){ //message);
        printf("Communicable %d\n", a);
    }
};

#endif
