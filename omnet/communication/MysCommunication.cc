#include "MysCommunication.h"

#include <thread>
#include "TaskMessage.h"
#include "../../src/communication/Communication.h"
#include "UAVRegistry.cc"

using namespace std;
//Enviar mensagens Unicast, Broadcast e Multicast

int ct = -1; //Passar o Array pro Framework

int MysCommunication::configureSocketServer(int port){
    return Communication::configureSocketServer(port);
}

void MysCommunication::sendMessageToUAV(int id, Message msg){
    Communication::sendMessageToUAV(id, msg);
}

void MysCommunication::sendTaskMessageToUAV(int idSocket, TaskMessage tmsg){
    cout << "Criando tarefa com id: " << tmsg.getTask().getID() << endl;
    thread enviar(MessageSender(), idSocket, tmsg);
    enviar.detach();
}

//status->subscribe(newUAV);?
/*void CommunicationSocket::listenSocket(){
    Communication::listenSocket();
}*/

int* MysCommunication::getActiveConnections(){
    return nullptr;//Communication::conexoes; //conexoes;
}

void MysCommunication::ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg){
    getActiveConnections();
    if(msg.getCode() == 11)
        dest->onMessageReceive(msg);

    //SpecificClass* s = dynamic_cast<SpecificClass*>(dest);
}
