#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../utils/Message.h"
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
    virtual int configureSocketServer(int port){
        if(port < 1000){
            std::cerr << "Inaccessible ports" << std::endl;
            exit(0);
        }

        char msg[1500];

        sockaddr_in serverAddress;
        bzero((char*)&serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(port);

        int serverSd = socket(AF_INET, SOCK_STREAM, 0);
        if(serverSd < 0){
            cerr << "Error establishing the server socket" << std::endl;
            exit(0);
        }

        int bindStatus = bind(serverSd, (struct sockaddr*) &serverAddress,
            sizeof(serverAddress));
        if(bindStatus < 0){
            std::cerr << "Error binding socket to local address" << std::endl;
            exit(0);
        }
        std::cout << "Waiting for a UAV to connect..." << std::endl;
        listen(serverSd, 5);
        return serverSd;
    }
    virtual void sendMessageSocket(){
        std::cout << "Digite um UAV para receber" << std::endl;
        while(true){
            int id;
            std::cin >> id;
            if(id == -1){ //Broadcast
                std::cout << "Digite a mensagem:" << std::endl;
                char m[1500];
                std::cin >> m;
                for (int i = 0; i <= ct; i++){
                    MysMessage msg;
                    msg.setCode(10);
                    msg.setMsg(m);
                    msg.setDestination(i);
                    msg.setSource(-1);
                    thread enviar(SendSocket(), conexoes[i], msg);
                    enviar.detach();
                }
            }else{ //unicast
                thread enviar(SendSocket(), conexoes[id]);
                enviar.join();
            }
        }
    }
    virtual void listenSocket(){

        int serverSd = configureSocketServer(PORT);
        thread conectar(ConnSocket(), serverSd, conexoes);
        while(ct == -1){

        }
        cout << "Passando por Socket Communication" << endl;
        int antigo = 1;
        if(antigo == 0){
            while(1){
                thread enviar(SendSocket(), conexoes[0]);
                enviar.join();
            }
        }else{
            sendMessageSocket();
        }

        conectar.join();
    }
//protected:
};

#endif
