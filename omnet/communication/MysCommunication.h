#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_

#include "../../src/utils/Coordinate.h"
#include "../../src/communication/Communication.h"
#include "../../src/communication/Communicable.h"
#include "../../src/utils/Message.h"
#include "../status/MysStatus.h"

// componente de comunicação do framework com os drones
class MysCommunication : public Communication {
public:

    //Communication
    void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg);

    int* getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    //void listenSocket();
    void sendMessageToUAV(int id, Message msg);

    int configureSocketServer(int port);

    void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg);

    //int conexoes[NUMUAVS];
};

#endif