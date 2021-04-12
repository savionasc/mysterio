#include "Comm.h"

#include <mysterio/src/mysterio/Mysterio.h>
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

//Mostrar opções sincronas e assincronas / Alguma forma de Enviar mensagens

//Ou criar um outra mensagem e Minha mensagem herdar dessa classe, ou mudar as coisas
#include "../../common/msg/MinhaMensagem_m.h"
#include <mysterio/src/status/Status.h>
#include "Loko.h"

typedef std::map<int, MinhaMensagem> MyMap;

//Isso é certo?!
/*void Communicable::onMessageReceive(){
}*/
//void Comm::onMessageReceive(){
//}

void Comm::sendMessage(Communicable *source, Communicable *dest, int msg){
    if(msg == 0){
        dest->onMessageReceive(msg);
        //Loko* lok = dynamic_cast<Loko*>(dest);
        //lok->onMessageReceive(msg);
    }
}

Comm::Comm() {
    // TODO Auto-generated constructor stub
}

Comm::~Comm() {
    // TODO Auto-generated destructor stub
}

void Comm::sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg){
    messages[idDestination] = *msg;
}

bool Comm::hasMessageToDrone(int destination){
    std::map<int,MinhaMensagem>::iterator it = messages.find(destination);
    //if(it->first == destination)
    if(it != messages.end())
        return true;
    else
        return false;
}

int Comm::hasMessages(){
    return messages.size();
}

std::queue<int> Comm::messagesToSend(){
    std::queue<int> fila;
    for (std::map<int, MinhaMensagem>::iterator it=messages.begin(); it!=messages.end(); ++it)
        fila.push(it->first);
    return fila;
}

MinhaMensagem Comm::receiveMessage(int destination){
    return messages[destination];
}

void Comm::markAsReceived(int destination){
    messages.erase(destination);
}

void Comm::connectANewUAV(int ID, Status *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void Comm::disconnectUAV(int ID, Status *aggregator){
    aggregator->removeUAV(ID);
}

void Comm::saveUAVCurrentPosition(int idUAV, double x, double y, double z, Status *aggregator){
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    coord.z = z;
    aggregator->setUAVLocation(coord, idUAV);
}

void Comm::saveUAVCurrentPosition(int idUAV, Coordinate coord, Status *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate Comm::requestUAVCurrentPosition(int idUAV, Status *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void Comm::saveUAVCurrentVelocity(int idUAV, double velocity, Status *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double Comm::requestUAVCurrentVelocity(int idUAV, Status *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}

/*void Communication::setNumNodes(int nodes, Aggregator* aggregator){
    aggregator->numNodes = nodes;
}

int Communication::getNumNodes(Aggregator* aggregator){
    return aggregator->numNodes;
}*/
