#include "MysCommunication.h"

//#include <socket>
#include "UAVRegistry.cc"
#include "../status/MysStatusManager.h"
#include <thread>
#include "../../src/communication/Communication.h"
#include "../../src/communication/TaskMessage.h"

using namespace std;
//Enviar mensagens Unicast, Broadcast e Multicast

int MysCommunication::configureSocketServer(int portServer){
    //return Communication::configureSocketServer(port);

    //Here is configured and inicialized the socket server
    //Returns -1 for Inaccessible ports
    //Returns -2 for Error establishing the server socket
    //Returns -3 for Error binding socket to local address
    //Returns positivo for Waiting for a UAV to connect
    if(portServer < 1000)
       return -1;

    sockaddr_in serverAddress;
    bzero((char*)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(portServer);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
       return -2;

    int bindStatus = bind(serverSd, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
    if(bindStatus < 0)
       return -3;

    listen(serverSd, 5);
    return serverSd;
}

void MysCommunication::sendMessageToUAV(int idUAV, Message msg){
    MysStatusManager *ms;
    if(idUAV == -1){ //Broadcast
        cout << "BROADCAST" << endl;
        for (int i = 0; i <= ms->getSize(); i++){
            msg.setDestination(i);
            thread enviar(MessageSender(), ms->getUAV(i).getIdSocket(), msg);
            enviar.detach();
        }
    }else{ //unicast
        thread enviar(MessageSender(), ms->getUAV(idUAV).getIdSocket(), msg);
        enviar.join();
    }
}

void MysCommunication::sendTaskMessageToUAV(int idSocket, TaskMessage tmsg){
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
