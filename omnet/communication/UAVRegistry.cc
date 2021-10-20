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
//extern int conexoes[];
extern int ct;

class UAVRegistry{
public:
    void operator()(int param1, int *param2){
        while(conectarNovoUAV(param1, param2)){ }
    }

    bool conectarNovoUAV(int serverSd, int *conexoes){
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSistdze = sizeof(newSockAddr);
        int idSocket = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSistdze);
        if(idSocket < 0){
            std::cerr << "Error accepting request from UAV!" << std::endl;
            exit(1);
            return false;
        }
        std::cout << "Connected with UAV!" << std::endl;
        ct++;
        MysStatus *ms;
        UAV u(ms->getSize());
        u.setIdSocket(idSocket);
        ms->addUAV(u);
        conexoes[ct] = idSocket;
        cout << "UAV registrado: " << u.getID() << " ct: " << ct << endl;
        thread conectar(UAVRegistry(), serverSd, conexoes);
        //thread receber(MessageReceive(), conexoes[ct]);
        thread receber(MessageReceive(), ms->getUAV(ct).getIdSocket());
        receber.join();
        return true;
    }
};
#endif

