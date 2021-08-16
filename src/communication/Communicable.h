#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_

#include "../utils/Message.h"

// deve ser implementado em todos os objetos comunicaveis (Componentes ou UAVs)
//This interface is important to create Communicables classes
//Communication Receive?
class Communicable { //Abstract Class?
public:
    virtual void onMessageReceive(Message msg){ }
};

#endif
