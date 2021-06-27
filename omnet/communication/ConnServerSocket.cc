#ifndef MYSTERIO_OMNET_COMMUNICATION_CONNSOCKET_H_
#define MYSTERIO_OMNET_COMMUNICATION_CONNSOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "ReceiveServerSocket.cc"
#include "SendServerSocket.cc"

using namespace std;
//extern int conexoes[];
extern int ct;

class ConnSocket{
public:
    void operator()(int param1, int *param2){
        while(conectarNovoUAV(param1, param2)){ }
    }

    bool conectarNovoUAV(int serverSd, int *conexoes){
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSistdze = sizeof(newSockAddr);
        int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSistdze);
        if(newSd < 0){
            std::cerr << "Error accepting request from UAV!" << std::endl;
            exit(1);
            return false;
        }
        std::cout << "Connected with UAV!" << std::endl;
        ct++;
        conexoes[ct] = newSd;
        thread conectar(ConnSocket(), serverSd, conexoes);
        thread receber(ReceiveSocket(), conexoes[ct]);
        receber.join();
        return true;
    }
};
#endif

