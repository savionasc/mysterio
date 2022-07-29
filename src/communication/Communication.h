#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include <thread>
#include "Message.h"
#include "StatusMessage.h"
#include "TaskMessage.h"
#include "Communicable.h"
#include "../mission/MissionPlanner.h"

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication {
public:
    virtual bool connectUAV(UAV u) = 0;

    virtual void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg) = 0;

    virtual int countActiveConnections() = 0;

    virtual void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg) = 0;

    virtual int configureConnection(int connection) = 0;

    virtual void sendMessageToUAV(int idUAV, Message msg) = 0;

    virtual void reconnectUAV(UAV u) = 0;

    //virtual void setConfigure(Config configures) = 0;
    //Aqui criaria uma classe Config no utils que talvez fosse importantes para guardar IPServer, PORT...
};

#endif
