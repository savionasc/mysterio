#ifndef MYSTERIO_OMNET_COMMUNICATION_CONNSOCKET_H_
#define MYSTERIO_OMNET_COMMUNICATION_CONNSOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "MessageReceive.cc"
#include "MessageSender.cc"
#include "../status/MysStatus.h"

using namespace std;

class UAVRegistry{
public:
    //void operator()(int param1, int *param2){
        //while(conectarNovoUAV(param1, param2)){ }
    void operator()(int param){
        while(conectarNovoUAV(param)){ }
    }

    //bool conectarNovoUAV(int serverSocket, int *listaConexoes){
    bool conectarNovoUAV(int serverSocket){
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSistdze = sizeof(newSockAddr);
        int idSocket = accept(serverSocket, (sockaddr *)&newSockAddr, &newSockAddrSistdze);
        if(idSocket < 0){
            std::cerr << "Error accepting request from UAV!" << std::endl;
            exit(1);
            return false;
        }
        std::cout << "Connected with UAV!" << std::endl;
        MysStatus *ms;
        UAV u(ms->getSize());
        u.setIdSocket(idSocket);
        ms->addUAV(u);
        //listaConexoes[u.getID()] = idSocket;
        cout << "UAV registrado: " << u.getID() << endl;
        thread conectar(UAVRegistry(), serverSocket);
        //thread receber(MessageReceive(), ms->getUAV(u.getID()).getIdSocket());
        thread receber(MessageReceive(), u.getIdSocket());
        receber.join();
        return true;
    }
};
#endif

