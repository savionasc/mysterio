#ifndef MYSTERIO_OCTOPUS_COMMUNICATION_MYSCOMMUNICATION_H_
#define MYSTERIO_OCTOPUS_COMMUNICATION_MYSCOMMUNICATION_H_

#include "../../src/utils/Coordinate.h"
#include "../../src/communication/Communication.h"
#include "../../src/communication/Communicable.h"
#include "../../src/communication/Message.h"
#include "../status/MysStatusManager.h"
#include "messagestruct.h"

// componente de comunicação do framework com os drones
class MysCommunication : public Communication {
public:

    //Communication
    void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg);

    int* getActiveConnections();
    //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    //void listenSocket();
    void sendMessageToUAV(int id, Message msg);

    int configureConnection(int connection){ return connection; }

    int countActiveConnections(){
        MysStatusManager *ms;
        return ms->getSize();
    }

    int configureSocketServer(int port);

    void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg);
    void sendMessagestkToUAV(int idUAV, message *msg);

    thread listenForNewConnections();

    void reconnect(UAV u){
    }

    int getPortServer() {
        return portServer;
    }

    void setPortServer(int portServer) {
        this->portServer = portServer;
    }

    //int conexoes[NUMUAVS];
private:
    int portServer;
};

#endif
