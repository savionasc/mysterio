#ifndef MYSTERIO_OMNET_EXCOMM2_COMMUNICATION_COMM2_H_
#define MYSTERIO_OMNET_EXCOMM2_COMMUNICATION_COMM2_H_
#include <mysterio/src/utils/Coordinate.h>
#include <map>
#include <queue>

#include <mysterio/src/status/Status.h>
#include <mysterio/src/communication/Communication.h>
#include <mysterio/src/communication/Communicable.h>
#include "../../common/msg/MinhaMensagem_m.h"

// componente de comunicação do framework com os drones
class Comm2 : public Communication {
public:
    Comm2();
    virtual ~Comm2();
    //Definir um tipo message
    void sendMessage(Communicable *source, Communicable *dest, int msg); //return void or bool
        //save or request?
    //save or request?

    //STATUS
    void saveUAVCurrentPosition(int idUAV, double x, double y, double z, Status *aggregator);
    void saveUAVCurrentPosition(int idUAV, Coordinate coord, Status *aggregator);
    Coordinate requestUAVCurrentPosition(int idUAV, Status *aggregator);
    void saveUAVCurrentVelocity(int idUAV, double velocity, Status *aggregator);
    double requestUAVCurrentVelocity(int idUAV, Status *aggregator);
    //Ou usar esse?
    //virtual void AtualizarDadosDoAgregador(int idUAV, Status s, Aggregator *aggregator);

    //MESSAGES
    virtual void sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg);//IMPLEMENTATION
    virtual bool hasMessageToDrone(int destination);
    virtual int hasMessages();
    std::queue<int> messagesToSend(); //Provavelmente isso é implementação


    virtual void markAsReceived(int destination);
    virtual void connectANewUAV(int ID, Status *aggregator);
    virtual void disconnectUAV(int ID, Status *aggregator);
    //virtual int getNumNodes(Aggregator* aggregator);
    //virtual void setNumNodes(int nodes, Aggregator* aggregator);

    //Mudar isso, é do Omnet++
    MinhaMensagem receiveMessage(int destination); //IMPLEMENTATION
private:
    std::map<int,MinhaMensagem> messages;
};

#endif /* MYSTERIO_BUILDING_F_COMMUNICATION_H_ */
