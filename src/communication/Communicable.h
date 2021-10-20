#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_

#include "Message.h"
#include "StatusMessage.h"
#include "TaskMessage.h"

// deve ser implementado em todos os objetos comunicaveis (Componentes ou UAVs)
//This interface is important to create Communicables classes
//Communication Receive?
class Communicable { //Abstract Class?
public:
    virtual void onMessageReceive(Message msg){ }

    virtual void onStatusMessageReceive(StatusMessage msg){ }

    virtual void onTaskMessageReceive(TaskMessage msg){ }
};

#endif
