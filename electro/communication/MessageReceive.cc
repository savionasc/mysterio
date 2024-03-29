#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../../src/communication/Message.h"
#include "../../src/communication/StatusMessage.h"
#include "../../src/communication/TaskMessage.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../status/MysStatusManager.h"
#include "MessageSender.cc"

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
            //cout << "MensagemStatus: " << msg.getCode() << "X" << msg.getStatus().getLocationX() << msg.getMsg() << endl;
            cout << "<SM>message: " << msg.getMsg() << endl;
            if(strcmp(msg.getMsg(), "velocity") == 0){
                MysStatusManager* status = MysStatusManager::GetInstance();
                cout << "Number of UAVs on singleton: " << status->numeroUAVs() << endl;
            }

            if(strcmp(msg.getMsg(), "location") == 0){
                cout << "Add on singleton: " << endl;
                MysStatusManager* status = MysStatusManager::GetInstance();
                status->addUAV();
            }

            if(!strcmp(msg.getMsg(), "exit")){
                std::cout << "UAV has quit the session" << std::endl;
                return false;
            }
            //else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            //Ideia de definir tipos por intervalos
            else if(msg.getCode() > 10 && msg.getCode() < 21){
                MysStatusManager* status = MysStatusManager::GetInstance();
                status->onDroneStatusMessageReceive(msg);
            }else {
                //MysStatus *status;
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
            cout << "<TM>Received Task - Code Message: " << msg.getCode();
            cout << " Status: " << msg.getTask().getStatus() << " Task ID: " << msg.getTask().getID() << endl;
            cout << " Coordinate: " << msg.getCoord().getX() << "|" << msg.getCoord().getY() << "|" << msg.getCoord().getZ() << endl;
            cout << "Finished? " << (msg.getTask().isComplete() == true ? "True" : "False") << endl;
            cout << "UAV " << msg.getTask().getUAV().getID() << endl;
            TaskManager t;
            MysStatusManager *ms;
            if(msg.getCode() == Message::TASK_EMERGENCY_BATTERY_LOW){
                //Pegando informações não modificadas da tarefa
                Task task = msg.getTask();
                msg.setCode(Message::TASK_MESSAGE);
                UAV u = ms->getUAV(1);
                Task gotopos(u, msg.getCoord());
                gotopos.setType(Task::GOTO);
                char conteudo[12] = "SUBSTITUIR";
                msg.setDestination(u.getID());
                msg.setMsg(conteudo);
                t.assignTask(gotopos, u);
                t.addTask(gotopos);
                msg.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
                MessageSender msgSender;
                msgSender.enviarTarefa(u.getNetworkConfigurations().getIdSocket(), msg);

                //task.setType(FLY_AROUND);
                cout << "SUBSTITUIR UAV[" << task.getUAV().getID() << "] type: " << task.getType() << endl;
                cout << "Coordenadas: " << msg.getCoord().getX() << "|" << msg.getCoord().getY() << "|" << msg.getCoord().getZ() << endl;
                cout << "Waypoint: " << task.getWaypoints() << endl;
                task.setUAV(u);
                //t.substituirUAV(msg); //no que tem UAVCommunication.send();
                t.assignTask(task, u);
                t.addTask(task);

                msg.setDestination(u.getID());
                msg.setMsg(conteudo);
                msg.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
                msgSender.enviarTarefa(u.getNetworkConfigurations().getIdSocket(), msg);
            }
            //t.setTask(msg.getTask());
        }else if(typeMSG == Message::TASK_COMPLETED_MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(socket, (Message*)&msg, sizeof(msg), 0);
            cout << "[U" << msg.getDestination() << "] Task Finished, code message: " << msg.getCode() << endl;
        }
        return true;
    }
};
