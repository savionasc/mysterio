#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../utils/Message.h"
#include "../../omnet/communication/DroneStatusMessage.h"
#include "../../omnet/communication/ConnServerSocket.cc"
#include <thread>
//#include "../taskmanager/TaskManager.h"
#define NUMUAVS 2
#define PORT 1111

extern int conexoes[NUMUAVS];

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication {
public:
//    int conexoes[NUMUAVS]; //Passar o Array pro Framework
//public:
    //ReceiveMessageFromUAV or Route
    //Antigo sendMessage
    virtual void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg) = 0;
    //virtual int* getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    virtual int* getActiveConnections(){
        return conexoes;
    }

    virtual void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg){
        cout << "Criando tarefa com id: " << tmsg.task.id << endl;
        thread enviar(SendServerSocket(), idSocket, tmsg);
        enviar.detach();
    }

    virtual int configureSocketServer(int portServer){ //Here is configured and inicialized the socket server
        //Returns -1 for Inaccessible ports
        //Returns -2 for Error establishing the server socket
        //Returns -3 for Error binding socket to local address
        //Returns positivo for Waiting for a UAV to connect
        if(portServer < 1000)
            return -1;

        sockaddr_in serverAddress;
        bzero((char*)&serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(portServer);

        int serverSd = socket(AF_INET, SOCK_STREAM, 0);
        if(serverSd < 0)
            return -2;

        int bindStatus = bind(serverSd, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
        if(bindStatus < 0)
            return -3;

        listen(serverSd, 5);
        return serverSd;
    }

    //sendMessageToUAV
    //Antigo sendMessageSocket
    virtual void sendMessageToUAV(int idUAV, Message msg){

        //Ver onde colocar o TaskManager
        //if task, adicionar no MissionPlanner (Singleton)

        if(idUAV == -1){ //Broadcast
            cout << "BROADCAST" << endl;
            for (int i = 0; i <= ct; i++){
                msg.setDestination(i);
                thread enviar(SendServerSocket(), conexoes[i], msg);
                enviar.detach();
            }
        }else{ //unicast
            cout << "UNICAST" << endl;
            thread enviar(SendServerSocket(), conexoes[idUAV], msg);
            enviar.join();
        }
    }
};

#endif
