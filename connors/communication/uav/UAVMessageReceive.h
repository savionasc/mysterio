#ifndef MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#define MYSTERIO_OMNET_COMMUNICATION_UAVMESSAGERECEIVE_H_
#include "../../../src/utils/UAV.h"
#include "../../uavs/UAVMobility.h"
#include "UAVDispatcher.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;
using namespace std;

extern int step;
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

        //Aqui eu vou receber o ponteiro de tarefas...
        void operator()(int param, UAV *u, int param3, int param5, std::vector<Task> *param4){//, int *param5){
            //this->uav->setID(param2);
            this->setUAV(u);
            NetworkConfigurations ntc = this->uav->getNetworkConfigurations();
            ntc.setIdSocket(param3);
            this->uav->setNetworkConfigurations(ntc);
            if(param5 == 15){
                cout << "Chegou aqui assim: " << param4->size() << endl;
                this->setTaskList(param4);
            }
            //this->identificacao = param5;
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
                    //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav->getID() << "] Sending status " << std::endl;
                    //Coord coor = position[this->uav->getID()];
                    Coordinate coor = this->uav->getStatus().getCoordinate();

                    UAVDispatcher u;
                    u.setSocketCode(this->uav->getNetworkConfigurations().getIdSocket());
                    u.setUAV(uav);
                    //u.setSelfID(this->uav->getID());

                    //MUDAR AQUI???
                    StatusMessage m(msg.getMsg(), LOCATION_STATUS_RESPONSE, this->uav->getID(), -1);
                    m.setSource(u.getUAV()->getID());
                    UAVStatus d = m.getStatus();
                    //d.setLocation(coor.x, coor.y, coor.z);
                    d.setLocation(coor);
                    m.setStatus(d);

                    //Nao usado mais
                    char snd[1500];
                    std::string txt = "MSG - Coordenates (" + to_string(coor.getX());
                    txt += "," + to_string(coor.getY());
                    txt += "," + to_string(coor.getZ()) + ")";
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
                    u.setUAV(this->uav);

                    //MUDAR AQUI???
                    StatusMessage m(msg.getMsg(), VELOCITY_STATUS_RESPONSE, this->uav->getID(), -1);
                    UAVStatus d = m.getStatus();
                    d.setVelocity(vel);
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
                }else if(!strcmp(msg.getMsg(), "Defined Task")){
                    //take off
                    //receber a tarefa...
                }else if(!strcmp(msg.getMsg(), "id")){
                    cout<<"ID"<<endl;
                    //cout << "humm: " << this->identificacao << endl;
                }else if(!strcmp(msg.getMsg(), "size")){
                    cout<<"size"<<endl;
                    cout << "Tamnho da lista: " << this->uavTaskList->size() << endl;
                }else if(!strcmp(msg.getMsg(), "aaa")){
                    //Coordinate targetPosition(1+this->uavTaskList->size(),2+this->uavTaskList->size(),3+this->uavTaskList->size());
                    Coordinate posit(1,2,3);
                    cout << "Coordenadas: " << posit.getX() << "|" << posit.getY() << "|" << posit.getZ() << endl;
                    cout << "UAV: " << uav->getID() << endl;
                    UAV c(uav->getID());
                    Task tpx(c, posit);
                    cout << "Tamnho da lista: " << this->uavTaskList->size() << endl;

                    uavTaskList->clear();
                    uavTaskList->push_back(tpx);
                }else if(!strcmp(msg.getMsg(), "a1")){
                    cout << "Tarefa da pos0 do UAV " << uavTaskList->at(0).getUAV().getID() << endl;
                    cout << "Coordenadas: " << uavTaskList->at(0).getTarget().getX() << "|" << uavTaskList->at(0).getTarget().getY() << "|" << uavTaskList->at(0).getTarget().getZ() << endl;
                }else if(!strcmp(msg.getMsg(), "a2")){
                    cout << "Tarefa da pos1: " << endl;
                    cout << "Coordenadas: " << uavTaskList->at(1).getTarget().getX() << "|" << uavTaskList->at(1).getTarget().getY() << "|" << uavTaskList->at(1).getTarget().getZ() << endl;
                }
                
                // else if(!strcmp(msg.getMsg(), "decolar")){ //take off
                //     for (int i = 0; i < NUMUAVS; i++) {
                //         Coordinate currentP(100.0,100.0,100.0);
                //         UAV u(i);
                //         Task gotoc(u, currentP);
                //         base[i].push_back(gotoc);
                //         int j = base[i].size()-1;
                //         base[i][j].type = 10;
                //         base[i][j].uav->setID(i);
                //         if(itera[i] < 0){
                //             itera[i]++;
                //         }
                //         cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
                //     }

                // }else if(!strcmp(msg.getMsg(), "carro")){
                //     int i = this->uav->getID();//idUAV;
                //     Coordinate currentP(300.0,420.0,90.0);
                //     UAV u(i);
                //     Task gotoc(u, currentP);
                //     base[i].push_back(gotoc);
                //     int j = base[i].size()-1;
                //     base[i][j].type = FLY_AROUND;
                //     base[i][j].uav->setID(i);
                //     if(itera[i] < 0){
                //         itera[i]++;
                //     }
                //     cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
                // }
                
                else if(!strcmp(msg.getMsg(), "task")){
                    cout << "Current Task: " << itera[this->uav->getID()] << endl;
                }else if(!strcmp(msg.getMsg(), "d")){
                    // VELHA
                    //for (int i = 0; i < tasksVector[this->uav->getID()].size(); i++) {
                    //    cout << "Status: " << tasksVector[0][i].getStatus() << endl;
                    //}
                    for (int i = 0; i < uavTaskList->size(); i++) {
                        cout << "Status: " << uavTaskList->at(i).getStatus() << endl;
                    }
                }else if(!strcmp(msg.getMsg(), "dar-volta")){
                }else if(!strcmp(msg.getMsg(), "retornar-base")){
                }else if(!strcmp(msg.getMsg(), "nextStep")){
                    step = 1;
                }else if(!strcmp(msg.getMsg(), "goto0")){
                }else if(!strcmp(msg.getMsg(), "goto1")){
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

                // VELHA
                int i = x.getUAV().getID();//idUAV;
                //tasksVector[i].push_back(x);
                //int j = tasksVector[i].size()-1;
                //tasksVector[i][j].setType(x.getType());
                //tasksVector[i][j].getUAV().setID(x.getUAV().getID());

                // NOVA
                uavTaskList->push_back(x);
                uavTaskList->at(uavTaskList->size()-1).setType(x.getType()); //Redundante?
                uavTaskList->at(uavTaskList->size()-1).getUAV().setID(x.getUAV().getID()); //Redundante?

                if(itera[i] < 0){
                    itera[i]++;
                }
                //cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
            }
            return true;
        }

        void setTaskList(std::vector<Task> *taskList){
            cout << "Ao setar: " << taskList->size() << endl;
            this->uavTaskList = taskList;
        }

        void meuSize(){
            cout << "Agora: " << this->uavTaskList->size() << endl;
        }

        void setUAV(UAV *u){
            this->uav = u;
        }
    private:
        //int *identificacao;
        UAV *uav;
        std::vector<Task> *uavTaskList;
};

}

#endif
