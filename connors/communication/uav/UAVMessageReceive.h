#ifndef MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#define MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#include "../../../src/utils/UAV.h"
#include "../../uavs/UAVMobility.h"
#include "UAVDispatcher.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;
using namespace std;

namespace mysterio {

enum codes{
    LOCATION_STATUS_RESPONSE = 310,
    VELOCITY_STATUS_RESPONSE,
    BATTERY_STATUS_RESPONSE,
    FLIGHTTIME_STATUS_RESPONSE,
};

class UAVMessageReceive {
    public:
        UAVMessageReceive(){}
        virtual ~UAVMessageReceive(){}

        void operator()(int param, UAV *u, int param3, std::vector<Task> *param4){
            this->setUAV(u);
            NetworkConfigurations ntc = this->uav->getNetworkConfigurations();
            ntc.setIdSocket(param3);
            this->uav->setNetworkConfigurations(ntc);
            this->setTaskList(param4);
            while(waitMessage(param)){ }
        }

        bool waitMessage(int socket){
            int typeMSG;
            memset(&typeMSG, 0, sizeof(typeMSG));
            recv(socket, (int*)&typeMSG, sizeof(typeMSG), 0);
            if(typeMSG == Message::MESSAGE){
                //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
                Message msg;
                memset(&msg, 0, sizeof(msg));
                //Mensagem de sinalização
                recv(socket, (Message*)&msg, sizeof(msg), 0);
                if(!strcmp(msg.getMsg(), "exit") || !strcmp(msg.getMsg(), "quit")){
                    std::cout << "UAV has quit the session" << std::endl;
                    close(socket);
                    return false;

                    //IMPORTANTE

                    //POR MOTIVOS DE ORGANIZAÇÃO, ENVIAR DO SOCKET PARA UMA CLASSE
                    //O SOCKET SÓ DEVE TRATAR DE SOCKET E COMUNICAÇÃO
                    //A CLASSE QUE TRATA O QUE O FRAMEWORK QUER E DEVOLVE POR DISPATCH

                    //Daqui pra baixo enviar para o UAVCommunicationSocket
                    //Aí no OnMessageReceve ele trata tudo e fica mais organizado


                }else if(!strcmp(msg.getMsg(), "location")){
                    std::cout << "[U" << this->uav->getID() << "] Sending status " << std::endl;
                    Coordinate coord = this->uav->getStatus().getCoordinate();

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setUAV(uav);

                    StatusMessage m(msg.getMsg(), LOCATION_STATUS_RESPONSE, this->uav->getID(), -1);
                    m.setSource(u.getUAV()->getID());
                    UAVStatus d = m.getStatus();
                    d.setLocation(coord);
                    m.setStatus(d);

                    cout << "MSG: " << m.getStatus().getLocationX() << "|" << m.getStatus().getLocationY() << "|" << m.getStatus().getLocationZ() << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "velocity")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav->getID() << "] Sending status " << std::endl;

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setUAV(this->uav);

                    //MUDAR AQUI???
                    StatusMessage m(msg.getMsg(), VELOCITY_STATUS_RESPONSE, this->uav->getID(), -1);
                    UAVStatus d = m.getStatus();
                    d.setVelocity(this->uav->getStatus().getVelocity());
                    m.setStatus(d);
                    m.setSource(u.getUAV()->getID());

                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "battery")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Battery status " << std::endl;

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setUAV(this->uav);

                    char snd[150];
                    std::string txt = "MSG battery[" + to_string(this->uav->getID()) + "]: " + to_string(this->uav->getStatus().getBattery());
                    strcpy(snd, txt.c_str());
                    StatusMessage m(snd, BATTERY_STATUS_RESPONSE, this->uav->getID(), -1);
                    m.getStatus().setBattery(this->uav->getStatus().getBattery());
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "flight-time")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Flight Time Status " << std::endl;

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setUAV(this->uav);

                    char snd[150];
                    std::string txt = "MSG flight time[" + to_string(this->uav->getID()) + "]: " + to_string(tempoVoo[this->uav->getID()]);
                    strcpy(snd, txt.c_str());
                    StatusMessage m(snd, FLIGHTTIME_STATUS_RESPONSE, this->uav->getID(), -1);
                    m.getStatus().setFlightTime((int) tempoVoo[this->uav->getID()]);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "start")){
                    ativo[msg.getDestination()] = true;
                    cout << msg.getMsg() << ":" << msg.getDestination() << endl;
                }else if(!strcmp(msg.getMsg(), "stop")){
                    ativo[msg.getDestination()] = false;
                    cout << msg.getMsg() << ":" << msg.getDestination() << endl;
                }else if(!strcmp(msg.getMsg(), "task")){
                    cout << "Current Task: " << itera[this->uav->getID()] << endl;
                }else if(!strcmp(msg.getMsg(), "d")){
                    for (int i = 0; i < uavTaskList->size(); i++) {
                        cout << "Status: " << uavTaskList->at(i).getStatus() << endl;
                    }
                }else{
                    std::cout << "Received Message["<< this->uav->getID() <<"]: " << msg.getMsg() << std::endl;
                }
            }else if(typeMSG == Message::TASK_MESSAGE){
                cout << "Task Message received" << endl;
                //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
                //TaskMessage tmsg;
                TaskMessage tmsg;
                memset(&tmsg, 0, sizeof(tmsg));
                //recv(socket, (TaskMessage*)&tmsg, sizeof(tmsg), 0);
                recv(socket, (TaskMessage*)&tmsg, sizeof(tmsg), 0);
                Task x = tmsg.getTask();
                // TaskMessage tmsg2;
                // memset(&tmsg2, 0, sizeof(tmsg2));
                // recv(socket, (TaskMessage*)&tmsg2, sizeof(tmsg2), 0);
                // cout << "Mensagem recebida2: tipo: " << tmsg2.task.type << " idUAV: " << tmsg2.task.uav->getID() << endl;

                int i = x.getUAV().getID();

                uavTaskList->push_back(x);
                uavTaskList->at(uavTaskList->size()-1).setType(x.getType()); //Redundante?
                uavTaskList->at(uavTaskList->size()-1).getUAV().setID(x.getUAV().getID()); //Redundante?

                if(itera[i] < 0){
                    itera[i]++;
                }
            }
            return true;
        }

        void setTaskList(std::vector<Task> *taskList){
            this->uavTaskList = taskList;
        }

        void setUAV(UAV *u){
            this->uav = u;
        }
    private:
        UAV *uav;
        std::vector<Task> *uavTaskList;
};

}

#endif
