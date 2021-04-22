#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_

#include "../../src/utils/Coordinate.h"
#include <queue>
#include "../status/StatusC1.h"
#include "../../src/communication/Communication.h"
#include "../../src/communication/Communicable.h"
#include "../../src/utils/Message.h"

#define MAXUAVS 20

// componente de comunicação do framework com os drones
class CommunicationSocket : public Communication, public Communicable {
public:

    CommunicationSocket();
    virtual ~CommunicationSocket();

    //Communicable
    void onMessageReceive(Message msg);

    //Colocar origem e destino (ambos communicable) na mensagem?
    //Communication
    void sendMessage(Communicable *source, Communicable *dest, Message msg);

    void getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    void listening();
    void envMensagem();

    int configurar(int port);

    //Descartar?
    void saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *aggregator);
    void saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *aggregator);
    Coordinate requestUAVCurrentPosition(int idUAV, StatusC1 *aggregator);
    void saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *aggregator);
    double requestUAVCurrentVelocity(int idUAV, StatusC1 *aggregator);

    virtual bool hasMessageToDrone(int destination);
    virtual int hasMessages();
    std::queue<int> messagesToSend(); //Provavelmente isso é implementação

    virtual void markAsReceived(int destination);
    virtual void connectANewUAV(int ID, StatusC1 *status);
    virtual void disconnectUAV(int ID, StatusC1 *status);
};

#endif
