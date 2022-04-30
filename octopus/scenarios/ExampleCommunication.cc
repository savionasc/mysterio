#include "../scenarios/ExampleCommunication.h"
#include <iostream>
#include "../communication/MysCommunication.h"
#include "../../src/database/RepositoryMySQL.h"
#include "../../src/mission/MissionPlanner.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../communication/UAVRegistry.cc"

#include "../../src/mission/Command.h"

#include <sys/socket.h>
//inet_addr
#include <arpa/inet.h>
//write
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/*#pragma pack(1)

typedef struct message_t {
    uint32_t idMessage;
    uint32_t typeMessage;
    uint32_t idUAV;
    float speed;
    float battery;
    float location_x;
    float location_y;
    float location_z;
} message;

#pragma pack()*/

void assignPreprogrammedTasks(int, MysStatusManager*, MysCommunication);

//Here starts the server communication
void listenCommunication(){

    int conexoesEsperadas = 1;
    MysStatusManager *ms;

    MysCommunication comm;
    //Port number
    int serverSocket = comm.configureSocketServer(1111);
    comm.setPortServer(serverSocket);
    if(serverSocket > 0){
        thread conectar = comm.listenForNewConnections();
        std::cout << "Waiting for a UAV to connect..." << std::endl;
        //waiting for the first UAV
        while(ms->getSize() < conexoesEsperadas){
        }

        assignPreprogrammedTasks(conexoesEsperadas, ms, comm);

        //Menu
        while(true){
            std::cout << "Select a UAV by id:" << std::endl;
            int id;
            std::cin >> id;
            TaskManager t;
            UAV u(id);
            cout << "Number of tasks assigned to selected UAV: " << t.getNumTasks(u) << endl;
            std::cout << "Type a message:" << std::endl;
            char m[1500];
            std::cin >> m;
            Message msg(m, 10, -1, id);

            if(!strcmp(msg.getMsg(), "stattus")){
                std::cout << "select type message:" << std::endl;
                int typeMSG;
                std::cin >> typeMSG;
                int BUFFSIZE = 512;
                char buff[BUFFSIZE];
                bzero(buff, BUFFSIZE);
                message *a = (message*) buff;
                a->idMessage = 3;
                a->typeMessage = typeMSG;
                if(typeMSG == 11){
                    a->location_x  = 123;
                }else if(typeMSG == 12){
                    a->velocity  = 356.789;
                }else if(typeMSG == 13){
                    a->battery = 32.1;
                }
		        comm.sendMessagestkToUAV(0, a);
                //sendMsg(csock, a, sizeof(message));
            }
        }
        cout << "Join" << endl;
        conectar.join();
    }
    //when it fails enter here
    else{
        switch (serverSocket) {
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

void assignPreprogrammedTasks(int n, MysStatusManager *ms, MysCommunication comm){
    /*int BUFFSIZE = 512;
    char buff[BUFFSIZE];
    bzero(buff, BUFFSIZE);
    message *a = (message*) buff;
    a->idMessage = 3;
    a->typeMessage = 14;
    a->speed  = 356.789;
    sendMsg(csock, a, sizeof(message));*/
}

int main(int argc, char const *argv[]){
	//This number identification will be important to set database execution
    RepositoryMySQL rep;
    int numExecution = rep.createExecutionID();
    cout << "EXECUTION ID: " << numExecution << endl;

    listenCommunication();

    std::cout << "End execution" << std::endl;
    return 0;
}
