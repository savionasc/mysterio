#include "CommunicationSocket.h"

#include "../../../src/mysterio/Mysterio.h"
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

//Mostrar opções sincronas e assincronas / Alguma forma de Enviar mensagens

//Ou criar um outra mensagem e Minha mensagem herdar dessa classe, ou mudar as coisas
//#include "../../common/msg/MinhaMensagem_m.h"
#include "../../ExComm1/communication/Loko.h"

//typedef std::map<int, MinhaMensagem> MyMap;

//Isso é certo?!
/*void Communicable::onMessageReceive(){
}*/
//void Comm::onMessageReceive(){
//}

void CommunicationSocket::sendMessage(Communicable *source, Communicable *dest, int msg){
    if(msg == 0){
        dest->onMessageReceive(msg);
        //Loko* lok = dynamic_cast<Loko*>(dest);
        //lok->onMessageReceive(msg);
    }
}

void CommunicationSocket::testeMessage(){

}

CommunicationSocket::CommunicationSocket() { }

CommunicationSocket::~CommunicationSocket() { }

/*void Comm::sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg){
    messages[idDestination] = *msg;
}*/

bool CommunicationSocket::hasMessageToDrone(int destination){
    //std::map<int,MinhaMensagem>::iterator it = messages.find(destination);
    //if(it->first == destination)
    //if(it != messages.end())
    if(1)
        return true;
    else
        return false;
}

int CommunicationSocket::hasMessages(){
    return 1;//messages.size();
}

std::queue<int> CommunicationSocket::messagesToSend(){
    std::queue<int> fila;
    //for (std::map<int, MinhaMensagem>::iterator it=messages.begin(); it!=messages.end(); ++it)
        //fila.push(it->first);
    return fila;
}

/*MinhaMensagem Comm::receiveMessage(int destination){
    return messages[destination];
}*/

void CommunicationSocket::markAsReceived(int destination){
    //messages.erase(destination);
}

void CommunicationSocket::connectANewUAV(int ID, StatusC1 *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void CommunicationSocket::disconnectUAV(int ID, StatusC1 *aggregator){
    aggregator->removeUAV(ID);
}

void CommunicationSocket::saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *aggregator){
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    coord.z = z;
    aggregator->setUAVLocation(coord, idUAV);
}

void CommunicationSocket::saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate CommunicationSocket::requestUAVCurrentPosition(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void CommunicationSocket::saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double CommunicationSocket::requestUAVCurrentVelocity(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}

/*void Communication::setNumNodes(int nodes, Aggregator* aggregator){
    aggregator->numNodes = nodes;
}

int Communication::getNumNodes(Aggregator* aggregator){
    return aggregator->numNodes;
}*/
