#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../utils/Message.h"
#include "../../omnet/communication/DroneStatusMessage.h"
#include "../utils/Codes.h"
#include "../../omnet/communication/ConnServerSocket.cc"
#include <thread>

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication {
//protected:
public:
    int conexoes[NUMUAVS]; //Passar o Array pro Framework
public:
    virtual void sendMessage(Communicable *source, Communicable *dest, Message msg) = 0;
    //virtual int* getActiveConnections(); //Criar uma classe Conexões para guardar o ID do UAV e o Socket
    virtual int* getActiveConnections(){
        return conexoes;
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

<<<<<<< HEAD
    virtual void sendMessageSocket(int idUAV, Message msg){
        if(idUAV == -1){ //Broadcast
=======
    virtual void sendMessageSocket(int id, Message msg){//MysMessage msg){
        if(id == -1){ //Broadcast
>>>>>>> 6dd0609698aa40bee4636d8c2899f72df879ff70
            for (int i = 0; i <= ct; i++){
                msg.setDestination(i);
                thread enviar(SendSocket(), conexoes[i], msg);
                enviar.detach();
            }
        }else{ //unicast
            thread enviar(SendSocket(), conexoes[idUAV], msg);
            enviar.join();
        }
    }

    virtual void listenSocket(){ //Here starts the server communication
        int serverSd = configureSocketServer(PORT);
        if(serverSd > 0){
            thread conectar(ConnSocket(), serverSd, conexoes);
            std::cout << "Waiting for a UAV to connect..." << std::endl;
            while(ct == -1){ //waiting for the first UAV

            }
            while(true){
                std::cout << "Digite um UAV para receber" << std::endl;
                int id;
                std::cin >> id;
                std::cout << "Digite a mensagem:" << std::endl;
                char m[1500];
                std::cin >> m;
                Message msg(m, 10, -1, id);
                sendMessageSocket(id, msg);
            }
            conectar.join();
        }else{ //when it fails enter here
            switch (serverSd) {
                case -1:
                    std::cerr << "Inaccessible ports" << std::endl;
                    break;
                case -2:
                    std::cerr << "Error establishing the server socket" << std::endl;
                    break;
                case -3:
                    std::cerr << "Error binding socket to local address" << std::endl;
                    break;
            }
            exit(0);
        }
    }
};

#endif
