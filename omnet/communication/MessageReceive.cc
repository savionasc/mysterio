#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../../src/taskmanager/TaskManager.h"
#include "../status/MysStatus.h"
#include "../../src/utils/Message.h"
#include "DroneStatusMessage.h"
#include "TaskMessage.h"

using namespace std;

class MessageReceive{
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){
        int typeMSG;
        memset(&typeMSG, 0, sizeof(typeMSG));
        recv(newSd, (int*)&typeMSG, sizeof(typeMSG), 0);
        cout << "Tipo da mensagem: " << typeMSG << endl;
        if(typeMSG == STATUS_MESSAGE){
            DroneStatusMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (DroneStatusMessage*)&msg, sizeof(msg), 0);
            cout << "MensagemStatus: " << msg.getCode() << "X" << msg.getStatus().getLocationX() << msg.getMsg() << endl;
            cout << "Minha mensagem: " << msg.getMsg() << endl;
            if(strcmp(msg.getMsg(), "velocity") == 0){
                MysStatus* status = MysStatus::GetInstance();
                cout << "Numero de UAVs no singleton: " << status->numeroUAVs() << endl;
            }

            if(strcmp(msg.getMsg(), "location") == 0){
                cout << "Add no singleton: " << endl;
                MysStatus* status = MysStatus::GetInstance();
                status->addUAV();
            }

            if(!strcmp(msg.getMsg(), "exit")){
                std::cout << "UAV has quit the session" << std::endl;
                return false;
            }//else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            else if(msg.getCode() > 10 && msg.getCode() < 21){ //Ideia de definir tipos por intervalos
                MysStatus* status = MysStatus::GetInstance();
                status->onDroneStatusMessageReceive(msg);
            }else {
                //MysStatus *status;
                MysStatus* status = MysStatus::GetInstance();
                status->onDroneStatusMessageReceive(msg);
            }
        }else if(typeMSG == MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (Message*)&msg, sizeof(msg), 0);

            if(msg.getCode()){
                cout << "[U" << msg.getSource() << "] Atividade finalizada, codigo da mensagem: " << msg.getCode() << endl;
            }else{
                cout << "[U" << msg.getSource() << "] Mensagem recebida: " << msg.getMsg() << endl;
            }
            cout << "MSG: " << msg.getMsg() << endl;
        }else if(typeMSG == TASK_MESSAGE){
            TaskMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (TaskMessage*)&msg, sizeof(msg), 0);
            cout << "Tarefa recebida - Code Message: " << msg.getCode();
            cout << " Status: " << msg.getTask().getStatus() << " ID da tarefa: " << msg.getTask().getID() << endl;
            cout << "Finalizada? " << msg.getTask().isComplete() << endl;
            cout << "UAV " << msg.getTask().getUAV().getID() << endl;
            TaskManager t;
            t.setTask(msg.getTask());
        }else if(typeMSG == TASK_COMPLETED_MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (Message*)&msg, sizeof(msg), 0);
            cout << "[U" << msg.getDestination() << "] Atividade finalizada, codigo da mensagem: " << msg.getCode() << endl;
        }
        return true;
    }
};
