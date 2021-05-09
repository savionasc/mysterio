#include "CommunicationSocket.h"
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
int conexoes[MAXUAVS], ct = -1;

//Há problema se deixar aqui?
class socket_receber {
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){
        //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
        char msg[1500];
        memset(&msg, 0, sizeof(msg));
        recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit")){
            std::cout << "UAV has quit the session" << std::endl;
            return false;
        }else if(!strcmp(msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            StatusC1 status;
            Message m;
            status.onMessageReceive(m);
        }
        std::cout << "UAV Message: " << msg << std::endl;
        return true;
    }
};

class socket_enviar {
public:
    void operator()(int param){
        enviarResposta(param);
        //while(enviarResposta(param)){ }
    }

    void operator()(int param, char *param2){
        send(param, (char*)param2, strlen(param2), 0);
    }

    bool enviarResposta(int newSd){
        char msg[1500];
        std::cout << ">";
        std::string data;
        cin >> data;
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
        //if(data == "exit")
        send(newSd, (char*)&msg, strlen(msg), 0);
    }
};

class socket_conectar {
public:
    void operator()(int param){
        while(conectarNovoUAV(param)){ }
    }

    bool conectarNovoUAV(int serverSd){
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
};

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
    std::cout << "Digite um UAV para receber" << std::endl;
    while(true){
        int id;
        std::cin >> id;
        if(id == -1){ //Broadcast
            std::cout << "Digite a mensagem:" << std::endl;
            char m[1500];
            std::cin >> m;
            for (int i = 0; i <= ct; i++){
                thread enviar(socket_enviar(), conexoes[i], m);
                enviar.detach(); //Tava Join
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
    int antigo = 1;
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

void CommunicationSocket::onMessageReceive(Message msg){

}

void CommunicationSocket::sendMessage(Communicable *source, Communicable *dest, Message msg){
    getActiveConnections();
    if(msg.getCode() == 11){
        dest->onMessageReceive(msg);
        //Loko* lok = dynamic_cast<Loko*>(dest);
        //lok->onMessageReceive(msg);
    }
}

void CommunicationSocket::connectANewUAV(int ID, StatusC1 *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void CommunicationSocket::disconnectUAV(int ID, StatusC1 *aggregator){
    aggregator->removeUAV(ID);
}
