#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "ReceiveSocket.cc"
#include "SendSocket.cc"


using namespace std;
extern int conexoes[];
extern int ct;

///*
class ConnSocket{
public:
    void operator()(int param){
        while(conectarNovoUAV(param)){ }
    }

    bool conectarNovoUAV(int serverSd){
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
        thread conectar(ConnSocket(), serverSd);
        thread receber(ReceiveSocket(), conexoes[ct]);
        receber.join();
        return true;
    }
};//*/
