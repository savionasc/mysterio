#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_

#include "../../src/communication/UAVCommunication.h"
#include "../../src/utils/Message.h"
#include "CommunicationSocket.h"
#include <thread>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "inet/common/geometry/common/Coord.h"

using namespace inet;
using namespace std;

extern Coord position1[10];

namespace mysterio {

class UAVCommunicationSocket : public UAVCommunication {
    friend class socket_receber;
public:

    UAVCommunicationSocket();
    virtual ~UAVCommunicationSocket();

    //UAVCommunication
    void connectBase();
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
            char msg[1500];
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (char*)&msg, sizeof(msg), 0);
            if(!strcmp(msg, "exit") || !strcmp(msg, "quit")){
                std::cout << "UAV has quit the session" << std::endl;
                //UAVCommunication u; u.disconnectBase();
                close(newSd); //closesocket
                return false;
            }else if(!strcmp(msg, "location")){ //Mudar isso aqui e chamar o OnMessageReceve
                std::cout << " status " << std::endl;
                Coord coor = position1[idUAV];

                UAVCommunicationSocket u;
                u.setSocketCode(this->sock);
                u.setSelfID(this->idUAV);

                //Substituir isso por dispatchMessage

                char snd[150];
                std::string txt = "MSG: x: " + to_string(coor.x);
                txt += " y: " + to_string(coor.y);
                txt += " z: " + to_string(coor.z);
                strcpy(snd, txt.c_str());
                Message m(snd, 11);
                u.dispatchMessage(m);
            }
            std::cout << "Message to UAV[" << this->idUAV << "]: " << msg << std::endl;
            return true;
        }
    private:
        int idUAV;
        int sock;
};

}

#endif
