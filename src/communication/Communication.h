#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../status/Status.h"
#include "../utils/Message.h"
#include "../utils/Coordinate.h"
#include <map>
#include <queue>

//#include "../../omnet/common/msg/MinhaMensagem_m.h"

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication { //Interface
public:

    virtual void sendMessage(Communicable *source, Communicable *dest, Message msg) = 0;//char *mensagem //return void or bool

    //STATUS
    //void saveUAVCurrentPosition(int idUAV, double x, double y, double z, Status *aggregator);
    //void saveUAVCurrentPosition(int idUAV, Coordinate coord, Status *aggregator);
    //Coordinate requestUAVCurrentPosition(int idUAV, Status *aggregator);
    //void saveUAVCurrentVelocity(int idUAV, double velocity, Status *aggregator);
    //double requestUAVCurrentVelocity(int idUAV, Status *aggregator);
    //Ou usar esse?
    //virtual void AtualizarDadosDoAgregador(int idUAV, Status s, Aggregator *aggregator);

    //MESSAGES
    //virtual void sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg);//IMPLEMENTATION
    //virtual bool hasMessageToDrone(int destination);
    //virtual int hasMessages();
    //std::queue<int> messagesToSend(); //Provavelmente isso é implementação


    //virtual void markAsReceived(int destination);
    //virtual void connectANewUAV(int ID, Status *aggregator);
    //virtual void disconnectUAV(int ID, Status *aggregator);
    //virtual int getNumNodes(Aggregator* aggregator);
    //virtual void setNumNodes(int nodes, Aggregator* aggregator);

    //Mudar isso, é do Omnet++
    //MinhaMensagem receiveMessage(int destination); //IMPLEMENTATION
//private:
    //std::map<int,MinhaMensagem> messages;
};

#endif
