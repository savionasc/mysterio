#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#include "../../../src/utils/Coordinate.h"
#include <queue>
#include "../status/StatusC1.h"
#include "../../../src/communication/Communication.h"
#include "../../../src/communication/Communicable.h"
#include "../../../src/utils/Message.h"

// componente de comunicação do framework com os drones
class CommunicationSocket : public Communication, Communicable {
public:

    //Aqui vai extender de Communicable para ter o onMessageReceive()
    void onMessageReceive(Message msg); //Communicable
    //Daí o UAV vai usar UAVCommunicationSocket.sendMessage(MSG, Communicable destCommunication);
    //No sendMessage ele vai enviar via Socket

    CommunicationSocket();
    virtual ~CommunicationSocket();

    void getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket

    //Definir um tipo message //Colocar origem e destino (ambos communicable) na mensagem?
    void sendMessage(Communicable *source, Communicable *dest, Message msg); //return void or bool
    void testeMessage();
    void listening();
    void envMensagem();
    //void onMessageReceive();
    //save or request?

    //STATUS
    void saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *aggregator);
    void saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *aggregator);
    Coordinate requestUAVCurrentPosition(int idUAV, StatusC1 *aggregator);
    void saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *aggregator);
    double requestUAVCurrentVelocity(int idUAV, StatusC1 *aggregator);
    //Ou usar esse?
    //virtual void AtualizarDadosDoAgregador(int idUAV, StatusInformation s, Status *status);

    virtual bool hasMessageToDrone(int destination);
    virtual int hasMessages();
    std::queue<int> messagesToSend(); //Provavelmente isso é implementação

    virtual void markAsReceived(int destination);
    virtual void connectANewUAV(int ID, StatusC1 *status);
    virtual void disconnectUAV(int ID, StatusC1 *status);

    int configurar(int port);
    bool conectarNovoUAV(int serverSd);
    bool enviarResposta(int newSd);
    bool esperarMensagem(int newSd);
protected:
};

#endif
