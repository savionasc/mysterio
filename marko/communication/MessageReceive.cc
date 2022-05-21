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
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int socket){
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
                cout << "Number of UAVs on singleton: " << status->numeroUAVs() << endl;
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
            //else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            else if(msg.getCode() > 10 && msg.getCode() < 21){
                //Ideia de definir tipos por intervalos
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

            TaskManager t;
            MysStatusManager *ms;
            if(msg.getCode() == Message::TASK_EMERGENCY_BATTERY_LOW){
                Task task = msg.getTask();
                //Pegando informações não modificadas da tarefa
                msg.setCode(Message::TASK_MESSAGE);
                UAV u = ms->getUAV(1);
                Task gotopos(u, msg.getCoord());
                gotopos.setType(GOTO);
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
            }else if(msg.getCode() == Message::SUBORDINATE_SUBTASK){
                Coordinate targetPosition(msg.getCoord());
                UAV uav = ms->getUAV(msg.getDestination());
                Task subtask(uav, SURROUND_SHEEP, targetPosition);
                //Mudar esse código
                subtask.setPriority(5);

                char conteudo[12] = "URGENTE!";
                msg.setMsg(conteudo);
                t.assignTask(subtask, uav);
                t.addTask(subtask);
                msg.setTask(t.getTaskByIndex(uav, t.getNumTasks(uav)-1));
                MessageSender msgSender;
                UAV uavLeader = ms->getUAV(msg.getSource());
                msgSender.enviarTarefa(uavLeader.getNetworkConfigurations().getIdSocket(), msg);
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
