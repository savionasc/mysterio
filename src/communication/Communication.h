#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../utils/Message.h"

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication { //Interface
public:
    //return void or bool
    virtual void sendMessage(Communicable *source, Communicable *dest, Message msg) = 0;
};

#endif
