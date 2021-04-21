#include "UAVCommunicationSocket.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace mysterio {

UAVCommunicationSocket::UAVCommunicationSocket() { }

UAVCommunicationSocket::~UAVCommunicationSocket() { }

void UAVCommunicationSocket::dispatchMessage(Message msg){
    //Aqui envia mensagem via socket pro Communication
    send(this->getSocketCode(), msg.getMsg(), strlen(msg.getMsg()), 0);
}

void UAVCommunicationSocket::connectBase(){
    if(!this->connected){
        this->connected = true;
        int port = 1111;
        char serverIp[] = "127.0.0.1";
        char msg[1500];
        struct hostent* host = gethostbyname(serverIp);
        sockaddr_in sendSockAddr;
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(port);
        int clientSd = socket(AF_INET, SOCK_STREAM, 0);

        int status = connect(clientSd,
                             (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if(status < 0){
            std::cout<<"Error connecting to socket!"<<std::endl;
            this->connected = false;
        }else{
            std::cout << "Connected to the server!" << std::endl;
            this->connected = true;
            this->socketCode = clientSd;
        }


        /*while(1){
            std::cout << ">";
            std::string data;
            getline(std::cin, data);
            memset(&msg, 0, sizeof(msg));//clear the buffer
            strcpy(msg, data.c_str());
            if(data == "exit"){
                send(clientSd, (char*)&msg, strlen(msg), 0);
                break;
            }
            std::cout << "Awaiting server response..." << std::endl;
            memset(&msg, 0, sizeof(msg));//clear the buffer
            if(!strcmp(msg, "exit")){
                std::cout << "Server has quit the session" << std::endl;
                break;
            }
            std::cout << "Server: " << msg << std::endl;
        }
        close(clientSd);
        std::cout << "Connection closed" << std::endl;*/
    }
}

int UAVCommunicationSocket::getSocketCode(){
    return this->socketCode;
}

void UAVCommunicationSocket::disconnectBase(){
    close(getSocketCode());
}

}

