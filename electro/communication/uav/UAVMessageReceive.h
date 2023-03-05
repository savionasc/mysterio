#ifndef MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#define MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#include "../../../src/utils/UAV.h"
#include "../../uavs/UAVMobility.h"
#include "UAVDispatcher.h"

using namespace std;

extern int step;
namespace mysterio {

class UAVMessageReceive {
    public:
        UAVMessageReceive(){}
        virtual ~UAVMessageReceive(){}
        void operator()(int param, UAV *u, int param3, int param4){
            this->setUAV(u);
            this->uav->setID(param3);
            NetworkConfigurations ntc = this->uav->getNetworkConfigurations();
            ntc.setIdSocket(param4);
            this->uav->setNetworkConfigurations(ntc);
            //this->uav->setIdSocket(param3);
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
                recv(socket, (Message*)&msg, sizeof(msg), 0); //Mensagem de sinalização
                if(!strcmp(msg.getMsg(), "exit") || !strcmp(msg.getMsg(), "quit")){
                    std::cout << "UAV has quit the session" << std::endl;
                    close(socket);
                    return false;
                }else if(!strcmp(msg.getMsg(), "location")){ //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav->getID() << "] Sending status " << std::endl;
                    Coord coor = position[this->uav->getID()];

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setSelfID(this->uav->getID());

                    StatusMessage m(msg.getMsg(), LOCATION_STATUS_RESPONSE, this->uav->getID(), -1);   //MUDAR AQUI???
                    m.setSource(u.getSelfID());
                    UAVStatus d = m.getStatus();
                    d.setLocation(coor.x, coor.y, coor.z);
                    m.setStatus(d);

                    //Nao usado mais
                    char snd[1500];
                    std::string txt = "MSG - Coordenates (" + to_string(coor.x);
                    txt += "," + to_string(coor.y);
                    txt += "," + to_string(coor.z) + ")";
                    strcpy(snd, txt.c_str());
                    cout << "TXT: " << snd << endl;

                    cout << "MSG: " << m.getStatus().getLocationX() << "|" << m.getStatus().getLocationY() << "|" << m.getStatus().getLocationZ() << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "velocity")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav->getID() << "] Sending status " << std::endl;
                    double vel = velocidade[this->uav->getID()];

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setSelfID(this->uav->getID());

                    StatusMessage m(msg.getMsg(), VELOCITY_STATUS_RESPONSE, this->uav->getID(), -1); //MUDAR AQUI???
                    UAVStatus d = m.getStatus();
                    d.setVelocity(vel);
                    m.setStatus(d);
                    m.setSource(u.getSelfID());

                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "battery")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Battery status " << std::endl;

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setSelfID(this->uav->getID());

                    char snd[150];
                    std::string txt = "MSG battery[" + to_string(this->uav->getID()) + "]: " + to_string(bateria[this->uav->getID()]);
                    strcpy(snd, txt.c_str());
                    StatusMessage m(snd, BATTERY_STATUS_RESPONSE, this->uav->getID(), -1);
                    m.getStatus().setBattery(bateria[this->uav->getID()]);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "flight-time")){
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Flight Time Status " << std::endl;

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setSelfID(this->uav->getID());

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
                    for (int i = 0; i < tasksVector[this->uav->getID()].size(); i++) {
                        cout << "Status: " << tasksVector[0][i].getStatus() << endl;
                    }
                }else if(!strcmp(msg.getMsg(), "nextStep")){
                    step = 1;
                }else if(!strcmp(msg.getMsg(), "drenar")){
                    lowbattery[uav->getID()] = 1;
                }else{
                    std::cout << "Received Message["<< this->uav->getID() <<"]: " << msg.getMsg() << std::endl;
                }
            }else if(typeMSG == Message::TASK_MESSAGE){
                cout << "Task Message received" << endl;
                TaskMessage tmsg;
                memset(&tmsg, 0, sizeof(tmsg));
                recv(socket, (TaskMessage*)&tmsg, sizeof(tmsg), 0);
                if(!strcmp(tmsg.getMsg(), "SUBSTITUIR")){
                    ativo[uav->getID()] = true;
                }
                Task x = tmsg.getTask();
                int i = x.getUAV().getID();
                tasksVector[i].push_back(x);
                int j = tasksVector[i].size()-1;
                tasksVector[i][j].setType(x.getType());
                tasksVector[i][j].getUAV().setID(x.getUAV().getID());
                waypoints[i] = tasksVector[i][j].getWaypoints();
                if(itera[i] < 0){
                    itera[i]++;
                }
            }
            return true;
        }

        void setUAV(UAV *u){
            this->uav = u;
        }

    private:
        UAV *uav;
};
}

#endif
