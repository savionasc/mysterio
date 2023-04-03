#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICATION_H_
#include <thread>
#include "Message.h"
#include "StatusMessage.h"
#include "TaskMessage.h"
#include "Communicable.h"
#include "../mission/MissionPlanner.h"

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication {
public:
    //Antigo sendMessage
    virtual void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg) = 0;
    //virtual int* getActiveConnections(); retorna lista ou map de conexoes...

    virtual int countActiveConnections() = 0;

    virtual void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg) = 0;

    virtual int configureConnection(int connection) = 0;

    virtual int configureSocketServer(int portServer){ return true; }

    virtual void sendMessageToUAV(int idUAV, Message msg) = 0;

    virtual void reconnect(UAV u) = 0;

    //virtual void setConfigure(Config configures) = 0;
    //Aqui criaria uma classe Config no utils que talvez fosse importantes para guardar IPServer, PORT...
};

#endif
