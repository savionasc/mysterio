#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_

#include "../../src/utils/Coordinate.h"
#include "../status/StatusC1.h"
#include "../../src/communication/Communication.h"
#include "../../src/communication/Communicable.h"
#include "../../src/utils/Message.h"

// componente de comunicação do framework com os drones
class CommunicationSocket : public Communication, public Communicable {
public:

    //Communicable
    void onMessageReceive(Message msg);

    //Colocar origem e destino (ambos communicable) na mensagem?
    //Communication
    void sendMessage(Communicable *source, Communicable *dest, Message msg);

    void getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    void listening();
    void envMensagem();

    int configurar(int port);
};

#endif
