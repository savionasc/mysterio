#include "CommunicationSocket.h"
#include "../../../src/mysterio/Mysterio.h"
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast
//Ou criar uma classe mensagem

#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
int conexoes[5], ct = -1;

class socket_conectar {
public:
    void operator()(int param){
        CommunicationSocket cs;
        while(cs.conectarNovoUAV(param)){ }
    }
};

class socket_receber {
public:
    void operator()(int param){
        CommunicationSocket cs;
        while(cs.esperarMensagem(param)){ }
    }
};

class socket_enviar {
public:
    void operator()(int param){
        CommunicationSocket cs;
        while(cs.enviarResposta(param)){ }
    }
};

bool CommunicationSocket::esperarMensagem(int newSd){
    char msg[1500];
    memset(&msg, 0, sizeof(msg));
    recv(newSd, (char*)&msg, sizeof(msg), 0);
    if(!strcmp(msg, "exit")){
        std::cout << "UAV has quit the session" << std::endl;
        return false;
    }else if(!strcmp(msg, "status")){
        StatusC1 status;
        status.onMessageReceive(1);
    }
    std::cout << "UAV Message: " << msg << std::endl;
    return true;
}

bool CommunicationSocket::enviarResposta(int newSd){
    char msg[1500];
    std::cout << ">";
    std::string data;
    cin >> data;
    memset(&msg, 0, sizeof(msg));
    strcpy(msg, data.c_str());
    if(data == "exit"){
        send(newSd, (char*)&msg, strlen(msg), 0);
        return false;
    }
    send(newSd, (char*)&msg, strlen(msg), 0);
    return false;
}

bool CommunicationSocket::conectarNovoUAV(int serverSd){
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cerr << "Error accepting request from UAV!" << std::endl;
        exit(1);
        return false;
    }
    std::cout << "Connected with UAV!" << std::endl;
    ct++;
    conexoes[ct] = newSd;
    thread conectar(socket_conectar(), serverSd);
    thread receber(socket_receber(), conexoes[ct]);
    receber.join();
    return true;
}

int CommunicationSocket::configurar(int port){
    if(port < 1000){
        cerr << "Usage: port" << std::endl;
        exit(0);
    }

    char msg[1500];

    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
        sizeof(servAddr));
    if(bindStatus < 0){
        cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for a UAV to connect..." << std::endl;
    listen(serverSd, 5);
    return serverSd;
}

void CommunicationSocket::envMensagem(){
    std::cout << "Digite um numero para escolher quem vai receber" << std::endl;
    while(true){
        int id;
        cin >> id;
        if(id == -1){ //Broadcast
            for (int i = 0; i <= ct; i++){
                thread enviar(socket_enviar(), conexoes[i]);
                enviar.join();
            }
        }else{ //unicast
            thread enviar(socket_enviar(), conexoes[id]);
            enviar.join();
        }
        //enviar.joinable();
    }
}

void CommunicationSocket::listening(){

    int serverSd = configurar(1111);
    thread conectar(socket_conectar(), serverSd);
    while(ct == -1){

    }
    cout << "Passando por Socket Communication" << endl;
    int antigo = 0;
    if(antigo == 0){
        while(1){
            thread enviar(socket_enviar(), conexoes[0]);
            enviar.join();
        }
    }else{
        envMensagem();
    }

    conectar.join();
}

void CommunicationSocket::getActiveConnections(){

}

void CommunicationSocket::sendMessage(Communicable *source, Communicable *dest, int msg){
    getActiveConnections();
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

bool CommunicationSocket::hasMessageToDrone(int destination){
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

    coord.setX(x);
    coord.setY(y);
    coord.setZ(z);
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
