#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../status/MysStatus.h"
#include "../../src/utils/Message.h"
#include "DroneStatusMessage.h"

using namespace std;

class ReceiveSocket{
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){ //fazer os ifs para cada caso...

        int tipoMSG; //Recebendo tipo da mensagem para saber se é Status ou só Message mesmo...
        memset(&tipoMSG, 0, sizeof(tipoMSG));
        recv(newSd, (int*)&tipoMSG, sizeof(tipoMSG), 0);
        if(tipoMSG == -37){
            cout << "-37!!! " << tipoMSG << endl;
            DroneStatusMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (DroneStatusMessage*)&msg, sizeof(msg), 0);
            //Message msg2;
            //memset(&msg2, 0, sizeof(msg2));
            //recv(newSd, (Message*)&msg2, sizeof(msg2), 0);
            cout << "Mensagem: " << msg.msg << endl;
            //cout << "Message: " << msg2.msg << endl;
            //uMessage *msg = check_and_cast<uMessage*>(&msg);
            if(!strcmp(msg.msg, "exit")){
                std::cout << "UAV has quit the session" << std::endl;
                return false;
            }//else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            else if(msg.code > 10 && msg.code < 21){
                cout << "Criando Status1!" << endl;
                MysStatus status;
                status.onDroneStatusMessageReceive(msg);
            }else {
                cout << "Criando Status2!" << endl;
                MysStatus status;
                status.onDroneStatusMessageReceive(msg);
            }
            std::cout << "UAV Message: " << msg.status.getLocationX() << msg.status.getLocationY() << msg.status.getLocationZ() << std::endl;
        }else if(tipoMSG == -38){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (Message*)&msg, sizeof(msg), 0);
            cout << "Mensagem recebida!" << endl;
        }
        return true;
    }
};
