#include "CommunicationSocket.h"
#include "../../src/utils/Codes.h"
#include "../../src/communication/Communication.h"

#include <thread>

#include "ConnServerSocket.cc"

using namespace std;

//Enviar mensagens Unicast, Broadcast e Multicast

//int conexoes[NUMUAVS];
int ct = -1; //Passar o Array pro Framework

int CommunicationSocket::configureSocketServer(int port){
    return Communication::configureSocketServer(port);
    /*if(port < 1000){
        std::cerr << "Inaccessible ports" << std::endl;
        exit(0);
    }

    char msg[1500];

    sockaddr_in serverAddress;
    bzero((char*)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &serverAddress,
        sizeof(serverAddress));
    if(bindStatus < 0){
        std::cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for a UAV to connect..." << std::endl;
    listen(serverSd, 5);
    return serverSd;*/
}

void CommunicationSocket::sendMessageSocket(int id, Message msg){
    Communication::sendMessageSocket(id, msg);
    /*std::cout << "Digite um UAV para receber" << std::endl;
    while(true){
        int id;
        std::cin >> id;
        if(id == -1){ //Broadcast
            std::cout << "Digite a mensagem:" << std::endl;
            char m[1500];
            std::cin >> m;
            for (int i = 0; i <= ct; i++){
                MysMessage msg;
                msg.setCode(10);
                msg.setMsg(m);
                msg.setDestination(i);
                msg.setSource(-1);
                thread enviar(SendSocket(), conexoes[i], msg);
                enviar.detach();
            }
        }else{ //unicast
            thread enviar(SendSocket(), conexoes[id]);
            enviar.join();
        }
    }*/
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
