#include <thread>
#include "CommunicationSocket.h"
#include "../../src/communication/Communication.h"
#include "UAVRegistry.cc"

using namespace std;
//Enviar mensagens Unicast, Broadcast e Multicast

int ct = -1; //Passar o Array pro Framework

int CommunicationSocket::configureSocketServer(int port){
    return Communication::configureSocketServer(port);
}

void CommunicationSocket::sendMessageToUAV(int id, Message msg){
    Communication::sendMessageToUAV(id, msg);
}

//status->subscribe(newUAV);?
/*void CommunicationSocket::listenSocket(){
    Communication::listenSocket();
}*/

int* CommunicationSocket::getActiveConnections(){
    return nullptr;//Communication::conexoes; //conexoes;
}

void CommunicationSocket::ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg){
    getActiveConnections();
    if(msg.getCode() == 11)
        dest->onMessageReceive(msg);

    //SpecificClass* s = dynamic_cast<SpecificClass*>(dest);
}
