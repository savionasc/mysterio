#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#include <thread>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../src/communication/UAVCommunication.h"
#include "CommunicationSocket.h"
#include "../../src/utils/Message.h"
#include "../../omnet/communication/DroneStatusMessage.h"
#include "../communication/DroneStatusMessage.h"
#include "../../src/utils/Codes.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/power/base/EpEnergyStorageBase.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"

using namespace inet;
using namespace std;

extern Coord position[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
extern bool ativo[NUMUAVS];

namespace mysterio {

class UAVCommunicationSocket : public UAVCommunication {
    friend class socket_receber;
public:
    //UAVCommunication
    void connectBase();
    void dispatchStatusMessage(DroneStatusMessage msg);
    void dispatchMessage(Message msg); //Aqui ele deve enviar mensagem pro Communication
    void disconnectBase();

    int  conexao();
    int  getSocketCode();
    bool isConnected();
    void setConnected(bool connected);
    void setSocketCode(int socketCode);
    int  getSelfID();
    void setSelfID(int selfID);
protected:
    thread receber;
private:
    bool connected = false;
    int socketCode = -1;
    int selfID = -1;
};

class socket_receber {
    public:
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
                std::cout << " status " << std::endl;
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
