#ifndef MYSTERIO_ELECTRO_COMMUNICATION_UAVREGISTRY_H_
#define MYSTERIO_ELECTRO_COMMUNICATION_UAVREGISTRY_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "MessageReceive.cc"
#include "MessageSender.cc"
#include "../status/MysStatusManager.h"

using namespace std;

class UAVRegistry{
public:
    void operator()(int param){
        while(conectarNovoUAV(param)){ }
    }

    bool conectarNovoUAV(int serverSocket){
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSistdze = sizeof(newSockAddr);
        int idSocket = accept(serverSocket, (sockaddr *)&newSockAddr, &newSockAddrSistdze);
        if(idSocket < 0){
            std::cerr << "Error accepting request from UAV!" << std::endl;
            exit(1);
            return false;
        }
        MysStatusManager *ms;
        UAV u(ms->getSize());
        NetworkConfigurations ntc;
        ntc.setIdSocket(idSocket);
        u.setNetworkConfigurations(ntc);
        ms->addUAV(u);
        std::cout << "Connected with UAV[" << u.getID() << "]" << std::endl;
        thread conectar(UAVRegistry(), serverSocket);
        thread receber(MessageReceive(), u.getNetworkConfigurations().getIdSocket());
        receber.join();
        return true;
    }
};
#endif

