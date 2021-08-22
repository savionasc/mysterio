#ifndef MYSTERIO_OMNET_COMMUNICATION_SOCKETMESSAGERECEIVE_H_
#define MYSTERIO_OMNET_COMMUNICATION_SOCKETMESSAGERECEIVE_H_
#include "UAVCommunicationSocket.h"

extern int pular;
namespace mysterio {
class SocketMessageReceive {
    public:
        SocketMessageReceive(){}
        virtual ~SocketMessageReceive(){}
        void operator()(int param, int param2, int param3){
            this->idUAV = param2;
            this->sock = param3;
            while(esperarMensagem(param)){ }
        }

        bool esperarMensagem(int newSd){
            //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (Message*)&msg, sizeof(msg), 0);
            if(!strcmp(msg.getMsg(), "exit") || !strcmp(msg.getMsg(), "quit")){
                std::cout << "UAV has quit the session" << std::endl;
                close(newSd);
                return false;
            }else if(!strcmp(msg.getMsg(), "location")){ //Mudar isso aqui e chamar o OnMessageReceve
                std::cout << "[U" << this->idUAV << "] Respondendo status " << std::endl;
                Coord coor = position[idUAV];

                UAVCommunicationSocket u;
                u.setSocketCode(this->sock);
                u.setSelfID(this->idUAV);

                char snd[150];
                std::string txt = "MSG - Coordenadas (" + to_string(coor.x);
                txt += "," + to_string(coor.y);
                txt += "," + to_string(coor.z) + ")";
                strcpy(snd, txt.c_str());
                DroneStatusMessage m(snd, LOCATION_STATUS_RESPONSE, this->idUAV, -1);   //MUDAR AQUI???
                m.status.setLocation(coor.x, coor.y, coor.z);
                cout << txt << endl;
                u.dispatchStatusMessage(m);
            }else if(!strcmp(msg.getMsg(), "battery")){ //Mudar isso aqui e chamar o OnMessageReceve
                std::cout << " Battery status " << std::endl;

                UAVCommunicationSocket u;
                u.setSocketCode(this->sock);
                u.setSelfID(this->idUAV);

                char snd[150];
                std::string txt = "MSG bateria[" + to_string(this->idUAV) + "]: " + to_string(bateria[this->idUAV]);
                strcpy(snd, txt.c_str());
                DroneStatusMessage m(snd, BATTERY_STATUS_RESPONSE, this->idUAV, -1);
                m.status.setBattery(bateria[this->idUAV]);
                u.dispatchMessage(m);
            }else if(!strcmp(msg.getMsg(), "flight-time")){ //Mudar isso aqui e chamar o OnMessageReceve
                std::cout << " Flight Time Status " << std::endl;

                UAVCommunicationSocket u;
                u.setSocketCode(this->sock);
                u.setSelfID(this->idUAV);

                char snd[150];
                std::string txt = "MSG tempo de voo[" + to_string(this->idUAV) + "]: " + to_string(tempoVoo[this->idUAV]);
                strcpy(snd, txt.c_str());
                DroneStatusMessage m(snd, FLIGHTTIME_STATUS_RESPONSE, this->idUAV, -1);
                m.status.setFlightTime((int) tempoVoo[this->idUAV]);
                u.dispatchMessage(m);
            }else if(!strcmp(msg.getMsg(), "start")){
                ativo[msg.getDestination()] = true;
                cout << msg.getMsg() << ":" << msg.getDestination() << endl;
            }else if(!strcmp(msg.getMsg(), "stop")){
                ativo[msg.getDestination()] = false;
                cout << msg.getMsg() << ":" << msg.getDestination() << endl;
            }else if(!strcmp(msg.getMsg(), "decolar")){ //take off
                for (int i = 0; i < NUMUAVS; i++) {
                    Coordinate currentP(100.0,100.0,100.0);
                    GoTo gotoc(0, currentP);
                    base[i].push_back(&gotoc);
                    int j = base[i].size()-1;
                    base[i][j]->type = 10;
                    base[i][j]->idUAV = i;
                    //itera[i]++;
                    if(itera[i] < 0)
                        itera[i]++;
                    cout << "lista["<<i<<"]: " << base[i].size()<< endl;
                }

            }else if(!strcmp(msg.getMsg(), "carro")){
                int i = idUAV;
                //for (int i = 0; i < NUMUAVS; i++) {
                Coordinate currentP(300.0,420.0,90.0);
                Task gotoc(0, currentP);
                base[i].push_back(&gotoc);
                int j = base[i].size()-1;
                base[i][j]->type = FLY_AROUND;
                base[i][j]->idUAV = i;
                if(itera[i] < 0)
                    itera[i]++;
                cout << "lista["<<i<<"]: " << base[i].size()<< endl;
                //}
            }else if(!strcmp(msg.getMsg(), "dar-volta")){
            }else if(!strcmp(msg.getMsg(), "retornar-base")){
            }else if(!strcmp(msg.getMsg(), "next")){
                pular = 1;
            }else if(!strcmp(msg.getMsg(), "goto0")){
            }else if(!strcmp(msg.getMsg(), "goto1")){
            }else{
                std::cout << "Mensagem recebida em ["<< this->idUAV <<"]: " << msg.getMsg() << std::endl;
            }
            return true;
        }
    private:
        int idUAV;
        int sock;
};

}

#endif
