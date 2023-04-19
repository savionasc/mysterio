#ifndef MYSTERIO_ELECTRO_COMMUNICATION_UAV_UAVDISPATCHER_H_
#define MYSTERIO_ELECTRO_COMMUNICATION_UAV_UAVDISPATCHER_H_
#include <thread>
#include <iostream>
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
#include "../MysCommunication.h"

using namespace inet;
using namespace std;

namespace mysterio {

class UAVDispatcher : public UAVCommunication {
    friend class SocketMessageReceive;
public:
    UAVDispatcher(){}
    UAVDispatcher(UAV *u, std::vector<Task> *uavTasks){
        this->setUAV(u);
        this->setUAVTaskList(uavTasks);
    }

    void setUAVTaskList(std::vector<Task> *uavTasks){
        this->uavTasks = uavTasks;
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

    //UAVCommunication
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
    void setConnected(bool connected);
    void setSocketCode(int socketCode);
    UAV* getUAV();
    void setUAV(UAV *uav);
    int* getCurrentTask();
    void setCurrentTask(int *current);
protected:
    thread receber;
private:
    bool connected = false;
    int socketCode = -1;
    UAV *uav;
    std::vector<Task> *uavTasks;
    int *currentTask;
};
}

#endif
