#ifndef MYSTERIO_OMNET_COMMUNICATION_SOCKETMESSAGERECEIVE_H_
#define MYSTERIO_OMNET_COMMUNICATION_SOCKETMESSAGERECEIVE_H_
#include "UAVCommunicationSocket.h"
#include "../../src/utils/UAV.h"

extern int pular;
namespace mysterio {

enum codes{
    LOCATION_STATUS_RESPONSE = 310,
    VELOCITY_STATUS_RESPONSE,
    BATTERY_STATUS_RESPONSE,
    FLIGHTTIME_STATUS_RESPONSE,
};

class SocketMessageReceive {
    public:
        SocketMessageReceive(){}
        virtual ~SocketMessageReceive(){}
        void operator()(int param, int param2, int param3){
            this->uav.setID(param2);
            this->uav.setIdSocket(param3);
            while(esperarMensagem(param)){ }
        }

        bool esperarMensagem(int socket){
            int typeMSG;
            memset(&typeMSG, 0, sizeof(typeMSG));
            recv(socket, (int*)&typeMSG, sizeof(typeMSG), 0);
            if(typeMSG == MESSAGE){
                //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
                Message msg;
                memset(&msg, 0, sizeof(msg));
                recv(socket, (Message*)&msg, sizeof(msg), 0); //Mensagem de sinalização
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

                    //UAVCommunicationSocket u;
                    //u.OnMessageReceive(msg);
                    //IMPORTANTE


                }else if(!strcmp(msg.getMsg(), "location")){ //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav.getID() << "] Respondendo status " << std::endl;
                    Coord coor = position[this->uav.getID()];

                    UAVCommunicationSocket u;
                    u.setSocketCode(this->uav.getIdSocket());
                    u.setSelfID(this->uav.getID());

                    char snd[150];
                    std::string txt = "MSG - Coordenadas (" + to_string(coor.x);
                    txt += "," + to_string(coor.y);
                    txt += "," + to_string(coor.z) + ")";
                    strcpy(snd, txt.c_str());
                    DroneStatusMessage m(snd, LOCATION_STATUS_RESPONSE, this->uav.getID(), -1);   //MUDAR AQUI???
                    m.getStatus().setLocation(coor.x, coor.y, coor.z);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "velocity")){ //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << "[U" << this->uav.getID() << "] Respondendo status " << std::endl;
                    double vel = velocidade[this->uav.getID()];

                    UAVCommunicationSocket u;
                    u.setSocketCode(this->uav.getIdSocket());
                    u.setSelfID(this->uav.getID());

                    char snd[150];
                    std::string txt = "MSG - Velocidade (" + to_string(vel);
                    txt += "m/s)";
                    strcpy(snd, txt.c_str());
                    DroneStatusMessage m(snd, VELOCITY_STATUS_RESPONSE, this->uav.getID(), -1);   //MUDAR AQUI???
                    m.getStatus().setVelocity(vel);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "battery")){ //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Battery status " << std::endl;

                    UAVCommunicationSocket u;
                    u.setSocketCode(this->uav.getIdSocket());
                    u.setSelfID(this->uav.getID());

                    char snd[150];
                    std::string txt = "MSG bateria[" + to_string(this->uav.getID()) + "]: " + to_string(bateria[this->uav.getID()]);
                    strcpy(snd, txt.c_str());
                    DroneStatusMessage m(snd, BATTERY_STATUS_RESPONSE, this->uav.getID(), -1);
                    m.getStatus().setBattery(bateria[this->uav.getID()]);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "flight-time")){ //Mudar isso aqui e chamar o OnMessageReceve
                    std::cout << " Flight Time Status " << std::endl;

                    UAVCommunicationSocket u;
                    u.setSocketCode(this->uav.getIdSocket());
                    u.setSelfID(this->uav.getID());

                    char snd[150];
                    std::string txt = "MSG tempo de voo[" + to_string(this->uav.getID()) + "]: " + to_string(tempoVoo[this->uav.getID()]);
                    strcpy(snd, txt.c_str());
                    DroneStatusMessage m(snd, FLIGHTTIME_STATUS_RESPONSE, this->uav.getID(), -1);
                    m.getStatus().setFlightTime((int) tempoVoo[this->uav.getID()]);
                    cout << txt << endl;
                    u.dispatchStatusMessage(m);
                }else if(!strcmp(msg.getMsg(), "start")){
                    ativo[msg.getDestination()] = true;
                    cout << msg.getMsg() << ":" << msg.getDestination() << endl;
                }else if(!strcmp(msg.getMsg(), "stop")){
                    ativo[msg.getDestination()] = false;
                    cout << msg.getMsg() << ":" << msg.getDestination() << endl;
                }else if(!strcmp(msg.getMsg(), "tarefa definida")){ //take off
                    //receber a tarefa...
                }/*else if(!strcmp(msg.getMsg(), "decolar")){ //take off
                    for (int i = 0; i < NUMUAVS; i++) {
                        Coordinate currentP(100.0,100.0,100.0);
                        UAV u(i);
                        Task gotoc(u, currentP);
                        base[i].push_back(gotoc);
                        int j = base[i].size()-1;
                        base[i][j].type = 10;
                        base[i][j].uav.setID(i);
                        if(itera[i] < 0){
                            itera[i]++;
                        }
                        cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
                    }

                }else if(!strcmp(msg.getMsg(), "carro")){
                    int i = this->uav.getID();//idUAV;
                    Coordinate currentP(300.0,420.0,90.0);
                    UAV u(i);
                    Task gotoc(u, currentP);
                    base[i].push_back(gotoc);
                    int j = base[i].size()-1;
                    base[i][j].type = FLY_AROUND;
                    base[i][j].uav.setID(i);
                    if(itera[i] < 0){
                        itera[i]++;
                    }
                    cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
                }*/else if(!strcmp(msg.getMsg(), "task")){
                    cout << "Atividade atual: " << itera[this->uav.getID()] << endl;
                }else if(!strcmp(msg.getMsg(), "d")){
                    for (int i = 0; i < base[this->uav.getID()].size(); i++) {
                        cout << "Status: " << base[0][i].getStatus() << endl;
                    }
                }else if(!strcmp(msg.getMsg(), "dar-volta")){
                }else if(!strcmp(msg.getMsg(), "retornar-base")){
                }else if(!strcmp(msg.getMsg(), "next")){
                    pular = 1;
                }else if(!strcmp(msg.getMsg(), "goto0")){
                }else if(!strcmp(msg.getMsg(), "goto1")){
                }else{
                    std::cout << "Mensagem recebida em ["<< this->uav.getID() <<"]: " << msg.getMsg() << std::endl;
                }
            }else if(typeMSG == TASK_MESSAGE){
                cout << "Mensagem do tipo task" << endl;
                //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
                //TaskMessage tmsg;
                TaskMessage tmsg;
                memset(&tmsg, 0, sizeof(tmsg));
                //recv(socket, (TaskMessage*)&tmsg, sizeof(tmsg), 0);
                recv(socket, (TaskMessage*)&tmsg, sizeof(tmsg), 0);
                Task x = tmsg.getTask();
                cout << "Mensagem recebida1: tipo: " << tmsg.getCode() << endl;// << " idUAV: " << tmsg.uav.getID() << endl;
                cout << "Mensagem recebida2: tipo: " << tmsg.getCoord().getX() << " " << tmsg.getCoord().getY() << " " << tmsg.getCoord().getZ() << endl;
                cout << "Mensagem recebida3: tipo: " << x.getType() << " idUAV: " << x.getUAV().getID();
                cout << "Target: " << x.getTarget().getX() << " " << x.getTarget().getY() << " " << x.getTarget().getZ() << endl;
                /*TaskMessage tmsg2;
                memset(&tmsg2, 0, sizeof(tmsg2));
                recv(socket, (TaskMessage*)&tmsg2, sizeof(tmsg2), 0);
                cout << "Mensagem recebida2: tipo: " << tmsg2.task.type << " idUAV: " << tmsg2.task.uav.getID() << endl;*/
                int i = x.getUAV().getID();//idUAV;
                base[i].push_back(x);
                int j = base[i].size()-1;
                base[i][j].setType(x.getType());
                base[i][j].getUAV().setID(x.getUAV().getID());
                if(itera[i] < 0){
                    itera[i]++;
                }
                cout << "UAV["<<i<<"]-Tasks: " << base[i].size()<< endl;
            }
            return true;
        }
    private:
        UAV uav;
        //int idUAV;
        //int idSocket;
};

}

#endif
