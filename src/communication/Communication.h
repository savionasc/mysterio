#ifndef MYSTERIO_BUILDING_F_COMMUNICATION_H_
#define MYSTERIO_BUILDING_F_COMMUNICATION_H_
#include "Communicable.h"
#include "../utils/Message.h"
#include "../../omnet/communication/DroneStatusMessage.h"
#include "../../omnet/communication/ConnServerSocket.cc"
#include <thread>
#include "../../omnet/Singleton.h"

#define NUMUAVS 2
#define PORT    1111

// Componente de comunicação do framework com Classes do tipo Communicable
class Communication {
public:
    int conexoes[NUMUAVS]; //Passar o Array pro Framework
public:
    //ReceiveMessageFromUAV or Route
    //Antigo sendMessage
    virtual void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg) = 0;
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

    //sendMessageToUAV
    //Antigo sendMessageSocket
    virtual void sendMessageToUAV(int idUAV, Message msg){

        //Ver onde colocar o TaskManager
        //if task, adicionar no MissionPlanner (Singleton)

        if(!strcmp(msg.getMsg(), "task")){
            Singleton* singleton = Singleton::GetInstance("TASK");
            std::cout << singleton->value() << "\n";
            std::cout << "State: " << singleton->state() << "\n";
            singleton->SomeBusinessLogic();
            UAV u(0);
            std::cout << "Tarefas pro UAV0: " << singleton->numTasks(u) << "\n";
        }

        if(!strcmp(msg.getMsg(), "tasks")){
            Singleton* singleton = Singleton::GetInstance("TASK");
            std::cout << singleton->value() << "\n";
            std::cout << "State: " << singleton->state() << "\n";
            singleton->SomeBusinessLogic();
            UAV u(0);
            std::cout << "Tarefas pro UAV0: " << singleton->numTasks(u) << "\n";
            for (int i = 0; i < singleton->numTasks(u); i++) {
                cout << "Task: " << singleton->getTaskByIndex(u, i).type << endl;
                cout << "Status da tarefa: " << singleton->getTaskByIndex(u, i).status;
                cout << "ID da tarefa: " << singleton->getTaskByIndex(u, i).id << endl;
            }
        }

        if(!strcmp(msg.getMsg(), "decolar")){ //take off
            for (int i = 0; i < NUMUAVS; i++) {
                Coordinate currentP(100.0,100.0,100.0);
                UAV u(i);
                Task gotoc(u, currentP);
                gotoc.type = 10;
                gotoc.uav.setID(i);
                Singleton* singleton = Singleton::GetInstance("TASK");
                singleton->addTask(gotoc);
                cout << "UAV["<<u.getID()<<"]-Tasks: " << singleton->numTasks(u) << endl;
            }

        }else if(!strcmp(msg.getMsg(), "carro")){
            cout << "CARRO" << endl;
            Coordinate currentP(300.0,420.0,90.0);
            UAV u(idUAV);
            Task gotoc(u, currentP);
            gotoc.type = FLY_AROUND;
            gotoc.uav = u;
            Singleton* singleton = Singleton::GetInstance("TASK");
            singleton->addTask(gotoc);
            cout << "UAV["<<u.getID()<<"]-Tasks: " << singleton->numTasks(u) << endl;

            //Enviando tarefa
            int codeMessage = TASK_MESSAGE;
            TaskMessage taskMessage(msg.getMsg(), TASK_MESSAGE);
            taskMessage.c = singleton->getTaskByIndex(u, singleton->numTasks(u)-1).target;
            taskMessage.task = singleton->getTaskByIndex(u, singleton->numTasks(u)-1);
            cout << "Criando tarefa com id: " << taskMessage.task.id << endl;
            thread enviar(SendServerSocket(), conexoes[idUAV], taskMessage);
            enviar.detach();
        }else if(idUAV == -1){ //Broadcast
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

    virtual void listenSocket(){ //Here starts the server communication
        int serverSd = configureSocketServer(PORT);
        if(serverSd > 0){
            thread conectar(ConnServerSocket(), serverSd, conexoes);
            std::cout << "Waiting for a UAV to connect..." << std::endl;
            while(ct == -1){ //waiting for the first UAV

            }
            while(true){
                std::cout << "[C] Digite um UAV para receber:" << std::endl;
                int id;
                std::cin >> id;
                std::cout << "[C] Digite a mensagem:" << std::endl;
                char m[1500];
                std::cin >> m;
                Message msg(m, 10, -1, id);
                sendMessageToUAV(id, msg);
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
