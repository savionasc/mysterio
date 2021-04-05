#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include <mysterio/src/aggregator/Aggregator.h>
#include <mysterio/src/utils/Coordinate.h>
#include <map>
#include <queue>

#include "../../samples/common/msg/MinhaMensagem_m.h"

class Communication {
public:
    Communication();
    virtual ~Communication();
    //void saveInformationOnAggregator(int numero);
    //int requestInformationByAggregator();

    //STATUS
    void saveUAVCurrentPosition(int idUAV, double x, double y, double z, Aggregator *aggregator);
    void saveUAVCurrentPosition(int idUAV, Coordinate coord, Aggregator *aggregator);
    Coordinate requestUAVCurrentPosition(int idUAV, Aggregator *aggregator);
    void saveUAVCurrentVelocity(int idUAV, double velocity, Aggregator *aggregator);
    double requestUAVCurrentVelocity(int idUAV, Aggregator *aggregator);
    //Ou usar esse?
    //virtual void AtualizarDadosDoAgregador(int idUAV, Status s, Aggregator *aggregator);

    //MESSAGES
    virtual void sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg);//IMPLEMENTATION
    virtual bool hasMessageToDrone(int destination);
    virtual int hasMessages();
    std::queue<int> messagesToSend(); //Provavelmente isso é implementação


    virtual void markAsReceived(int destination);
    virtual void connectANewUAV(int ID, Aggregator *aggregator);
    virtual void disconnectUAV(int ID, Aggregator *aggregator);
    //virtual int getNumNodes(Aggregator* aggregator);
    //virtual void setNumNodes(int nodes, Aggregator* aggregator);

    //Mudar isso, é do Omnet++
    MinhaMensagem receiveMessage(int destination); //IMPLEMENTATION
private:
    std::map<int,MinhaMensagem> messages;
};

#endif /* MYSTERIO_BUILDING_F_COMMUNICATION_H_ */
