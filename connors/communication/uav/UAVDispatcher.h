#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../../../src/communication/Message.h"
#include "../../../src/communication/StatusMessage.h"
#include "../../../src/communication/TaskMessage.h"
#include "../../../src/communication/UAVCommunication.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/power/base/EpEnergyStorageBase.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"
#include "../../mission/GoTo.h"
#include "../MysCommunication.h"

using namespace inet;
using namespace std;

//extern em um vetor de UAV
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
//Tasks
//extern std::vector<Task> tasksVector[NUMUAVS];
extern bool ativo[NUMUAVS];
extern int itera[NUMUAVS];

namespace mysterio {

class UAVDispatcher: public UAVCommunication {
    friend class SocketMessageReceive;
public:
    //UAVCommunication
    UAVDispatcher(){}
    UAVDispatcher(UAV *u, std::vector<Task> *uavTasks){
        this->setUAV(u);
        this->setUAVTaskList(uavTasks);
    }

    void setUAVTaskList(std::vector<Task> *uavTasks){
        this->uavTasks = uavTasks;
    }

    int getCabrito(){
        return *cabrito;
    }


    void setCabrito(int *value){
        cabrito = value;
    }

    int changeCabrito(){
        int random = 1+(rand() % 100);
        *cabrito = random;
        return *cabrito;
    }

    void printTask(int id){
        std::cout << "conn-Tarefa-uav: " << uavTasks->at(id).getUAV().getID() << " tipo: " << uavTasks->at(id).getType() << " coordY? " << uavTasks->at(id).getTarget().getY() << std::endl;
    }

    void addNewTask(Task tarefa){
        uavTasks->push_back(tarefa);
    }

    void removeLastTask(){
        uavTasks->pop_back();
    }

    int listSize(){
        return uavTasks->size();
    }

    void connectBase();
    void dispatchTaskMessage(TaskMessage msg);
    void dispatchStatusMessage(StatusMessage msg);
    //Aqui ele deve enviar mensagem pro Communication
    void dispatchMessage(Message msg);
    void disconnectBase();
    void reconnect();

    int  conexao();
    int  getSocketCode();
    bool isConnected();
    void setConnected();
    void setDisconnected();
    void setSocketCode(int socketCode);
    //int  getSelfID();
    //void setSelfID(int selfID);
    UAV* getUAV();
    void setUAV(UAV *uav);


    int identificacao = 4;

protected:
    thread receber;
private:
    bool connected = false;
    int socketCode = -1;
    UAV *uav;
    //int selfID = -1;
    int *cabrito;
    std::vector<Task> *uavTasks;
};
}

#endif
