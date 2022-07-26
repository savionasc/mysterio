#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "MessageSender.cc"
#include "../../src/communication/Message.h"
#include "../../src/communication/StatusMessage.h"
#include "../../src/communication/TaskMessage.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../status/MysStatusManager.h"

using namespace std;

class MessageReceive{
public:
    void operator()(int param){
        while(waitMessage(param)){ }
    }

    bool waitMessage(int socket){
        int typeMSG;
        memset(&typeMSG, 0, sizeof(typeMSG));
        recv(socket, (int*)&typeMSG, sizeof(typeMSG), 0);

        if(typeMSG == Message::STATUS_MESSAGE){
            StatusMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(socket, (StatusMessage*)&msg, sizeof(msg), 0);
            cout << "<SM>message: " << msg.getMsg() << endl;
            if(strcmp(msg.getMsg(), "velocity") == 0){
                MysStatusManager* status = MysStatusManager::GetInstance();
                cout << "Number of UAVs on singleton: " << status->getNumberOfUAVs() << endl;
                cout << "MensagemStatus: " << msg.getCode() << " - " << msg.getStatus().getVelocity() << " - " << msg.getMsg() << endl;
            }

            if(strcmp(msg.getMsg(), "location") == 0){
                cout << "Add on singleton: " << endl;
                MysStatusManager* status = MysStatusManager::GetInstance();
                status->addUAV();
                cout << "MensagemStatus: " << msg.getCode() << " - " << msg.getStatus().getLocationX() << "|" << msg.getStatus().getLocationY() << "|" << msg.getStatus().getLocationZ() << " - " << msg.getMsg() << endl;
            }

            if(!strcmp(msg.getMsg(), "exit")){
                std::cout << "UAV has quit the session" << std::endl;
                return false;
            }
            else if(msg.getCode() > 10 && msg.getCode() < 21){
                MysStatusManager* status = MysStatusManager::GetInstance();
                status->onDroneStatusMessageReceive(msg);
            }else {
                MysStatusManager* status = MysStatusManager::GetInstance();
                status->onDroneStatusMessageReceive(msg);
            }
        }else if(typeMSG == Message::MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(socket, (Message*)&msg, sizeof(msg), 0);

            if(msg.getCode()){
                cout << "[U" << msg.getSource() << "] Task Finished, code message: " << msg.getCode() << endl;
            }else{
                cout << "[U" << msg.getSource() << "] Received Message: " << msg.getMsg() << endl;
            }
            cout << "<M>MSG: " << msg.getMsg() << endl;
        }else if(typeMSG == Message::TASK_MESSAGE){
            TaskMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(socket, (TaskMessage*)&msg, sizeof(msg), 0);

            TaskManager t;
            MysStatusManager *ms;
            if(msg.getCode() == Message::SUBORDINATE_SUBTASK){
                Coordinate targetPosition(msg.getCoord());
                UAV uav = ms->getUAV(msg.getDestination());
                Task subtask(uav, Task::SURROUND_SHEEP, targetPosition);
                subtask.setPriority(5);

                char content[12] = "URGENTE!";
                msg.setMsg(content);
                t.assignTask(subtask, uav);
                t.addTask(subtask);
                msg.setTask(t.getTaskByIndex(uav, t.getNumTasks(uav)-1));
                MessageSender msgSender;
                UAV uavLeader = ms->getUAV(msg.getSource());
                msgSender.sendTask(uavLeader.getNetworkConfigurations().getIdSocket(), msg);
            }
        }else if(typeMSG == Message::TASK_COMPLETED_MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(socket, (Message*)&msg, sizeof(msg), 0);
            cout << "[U" << msg.getDestination() << "] Task Finished, code message: " << msg.getCode() << endl;
        }

        return true;
    }
};
