#include <thread>
#include "CommunicationSocket.h"
#include "../../src/utils/Codes.h"
#include "../../src/communication/Communication.h"
#include "ConnServerSocket.cc"

using namespace std;

//Enviar mensagens Unicast, Broadcast e Multicast

//int conexoes[NUMUAVS];
int ct = -1; //Passar o Array pro Framework

int CommunicationSocket::configureSocketServer(int port){
    return Communication::configureSocketServer(port);
}

void CommunicationSocket::sendMessageSocket(int id, Message msg){
    Communication::sendMessageSocket(id, msg);
}

//status->subscribe(newUAV);?
void CommunicationSocket::listenSocket(){
    Communication::listenSocket();
    /*int serverSd = configureSocketServer(PORT);
    thread conectar(ConnSocket(), serverSd);
    while(ct == -1){

    }
    cout << "Passando por Socket Communication" << endl;
    int antigo = 1;
    if(antigo == 0){
        while(1){
            thread enviar(SendSocket(), conexoes[0]);
            enviar.join();
        }
    }else{
        sendMessageSocket();
    }

    conectar.join();*/
}

int* CommunicationSocket::getActiveConnections(){
    return nullptr;//Communication::conexoes; //conexoes;
}

void CommunicationSocket::sendMessage(Communicable *source, Communicable *dest, Message msg){
    getActiveConnections();
    if(msg.getCode() == 11)
        dest->onMessageReceive(msg);

    //SpecificClass* s = dynamic_cast<SpecificClass*>(dest);
}
