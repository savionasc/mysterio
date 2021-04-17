#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#include "../../../src/utils/Coordinate.h"
#include <map>
#include <queue>

#include "../status/StatusC1.h"
#include "../../../src/communication/Communication.h" //CommunicableImpl.h
//#include "../../common/msg/MinhaMensagem_m.h"

// componente de comunicação do framework com os drones
class CommunicationSocket : public Communication {
public:
    CommunicationSocket();
    virtual ~CommunicationSocket();

    //Definir um tipo message //Colocar origem e destino (ambos communicable) na mensagem?
    void sendMessage(Communicable *source, Communicable *dest, int msg); //return void or bool
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
    //virtual void AtualizarDadosDoAgregador(int idUAV, Status s, Aggregator *aggregator);

    //MESSAGES
    //ver virtual void sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg);//IMPLEMENTATION
    virtual bool hasMessageToDrone(int destination);
    virtual int hasMessages();
    std::queue<int> messagesToSend(); //Provavelmente isso é implementação


    virtual void markAsReceived(int destination);
    virtual void connectANewUAV(int ID, StatusC1 *aggregator);
    virtual void disconnectUAV(int ID, StatusC1 *aggregator);
    //virtual int getNumNodes(Aggregator* aggregator);
    //virtual void setNumNodes(int nodes, Aggregator* aggregator);

    //Mudar isso, é do Omnet++
    //Ver MinhaMensagem receiveMessage(int destination); //IMPLEMENTATION
    int configurar(int port);
    bool conectarNovoClient(int serverSd);
    bool enviarResposta(int newSd);
    bool esperarMensagem(int newSd);
protected:
    //Ver std::map<int,MinhaMensagem> messages;

};

#endif
