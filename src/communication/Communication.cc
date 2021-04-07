#include <mysterio/src/mysterio/Mysterio.h>
#include <mysterio/src/communication/Communication.h>

#include <mysterio/src/mysterio/Mysterio.h>
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

//Mostrar opções sincronas e assincronas / Alguma forma de Enviar mensagens

//Ou criar um outra mensagem e Minha mensagem herdar dessa classe, ou mudar as coisas
#include "../../samples/common/msg/MinhaMensagem_m.h"
#include "../status/Status.h"

typedef std::map<int, MinhaMensagem> MyMap;

Communication::Communication() {
    // TODO Auto-generated constructor stub
}

Communication::~Communication() {
    // TODO Auto-generated destructor stub
}

void Communication::sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg){
    messages[idDestination] = *msg;
}

bool Communication::hasMessageToDrone(int destination){
    std::map<int,MinhaMensagem>::iterator it = messages.find(destination);
    //if(it->first == destination)
    if(it != messages.end())
        return true;
    else
        return false;
}

int Communication::hasMessages(){
    return messages.size();
}

std::queue<int> Communication::messagesToSend(){
    std::queue<int> fila;
    for (std::map<int, MinhaMensagem>::iterator it=messages.begin(); it!=messages.end(); ++it)
        fila.push(it->first);
    return fila;
}

MinhaMensagem Communication::receiveMessage(int destination){
    return messages[destination];
}

void Communication::markAsReceived(int destination){
    messages.erase(destination);
}

void Communication::connectANewUAV(int ID, Status *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void Communication::disconnectUAV(int ID, Status *aggregator){
    aggregator->removeUAV(ID);
}

void Communication::saveUAVCurrentPosition(int idUAV, double x, double y, double z, Status *aggregator){
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    coord.z = z;
    aggregator->setUAVLocation(coord, idUAV);
}

void Communication::saveUAVCurrentPosition(int idUAV, Coordinate coord, Status *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate Communication::requestUAVCurrentPosition(int idUAV, Status *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void Communication::saveUAVCurrentVelocity(int idUAV, double velocity, Status *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double Communication::requestUAVCurrentVelocity(int idUAV, Status *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}

/*void Communication::setNumNodes(int nodes, Aggregator* aggregator){
    aggregator->numNodes = nodes;
}

int Communication::getNumNodes(Aggregator* aggregator){
    return aggregator->numNodes;
}*/
