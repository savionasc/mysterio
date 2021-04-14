#include "../../ExComm2/communication/Comm2.h"

#include <mysterio/src/mysterio/Mysterio.h>
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

//Mostrar opções sincronas e assincronas / Alguma forma de Enviar mensagens

//Ou criar um outra mensagem e Minha mensagem herdar dessa classe, ou mudar as coisas
#include <mysterio/src/status/Status.h>
#include "../../common/msg/MinhaMensagem_m.h"

typedef std::map<int, MinhaMensagem> MyMap;

//Isso é certo?!
void Comm2::sendMessage(Communicable *source, Communicable *dest, int msg){

}
/*void Communicable::onMessageReceive(){
}*/

Comm2::Comm2() {
    // TODO Auto-generated constructor stub
}

Comm2::~Comm2() {
    // TODO Auto-generated destructor stub
}

void Comm2::sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg){
    messages[idDestination] = *msg;
}

bool Comm2::hasMessageToDrone(int destination){
    std::map<int,MinhaMensagem>::iterator it = messages.find(destination);
    //if(it->first == destination)
    if(it != messages.end())
        return true;
    else
        return false;
}

int Comm2::hasMessages(){
    return messages.size();
}

std::queue<int> Comm2::messagesToSend(){
    std::queue<int> fila;
    for (std::map<int, MinhaMensagem>::iterator it=messages.begin(); it!=messages.end(); ++it)
        fila.push(it->first);
    return fila;
}

MinhaMensagem Comm2::receiveMessage(int destination){
    return messages[destination];
}

void Comm2::markAsReceived(int destination){
    messages.erase(destination);
}

void Comm2::connectANewUAV(int ID, Status *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void Comm2::disconnectUAV(int ID, Status *aggregator){
    aggregator->removeUAV(ID);
}

void Comm2::saveUAVCurrentPosition(int idUAV, double x, double y, double z, Status *aggregator){
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    coord.z = z;
    aggregator->setUAVLocation(coord, idUAV);
}

void Comm2::saveUAVCurrentPosition(int idUAV, Coordinate coord, Status *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate Comm2::requestUAVCurrentPosition(int idUAV, Status *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void Comm2::saveUAVCurrentVelocity(int idUAV, double velocity, Status *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double Comm2::requestUAVCurrentVelocity(int idUAV, Status *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}

/*void Communication::setNumNodes(int nodes, Aggregator* aggregator){
    aggregator->numNodes = nodes;
}

int Communication::getNumNodes(Aggregator* aggregator){
    return aggregator->numNodes;
}*/
