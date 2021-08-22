#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../../src/utils/Codes.h"
#include "../status/MysStatus.h"
#include "../../src/utils/Message.h"
#include "DroneStatusMessage.h"

using namespace std;
class ReceiveServerSocket{ //Não seria ReceiveUAVSocket?
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){
        int typeMSG;
        memset(&typeMSG, 0, sizeof(typeMSG));
        recv(newSd, (int*)&typeMSG, sizeof(typeMSG), 0);
        if(typeMSG == STATUS_MESSAGE){
            DroneStatusMessage msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (DroneStatusMessage*)&msg, sizeof(msg), 0);
            cout << "Mensagem: " << msg.msg << endl;
            if(!strcmp(msg.msg, "exit")){
                std::cout << "UAV has quit the session" << std::endl;
                return false;
            }//else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            else if(msg.code > 10 && msg.code < 21){ //Ideia de definir tipos por intervalos
                MysStatus status;
                status.onDroneStatusMessageReceive(msg);
            }else {
                MysStatus status;
                status.onDroneStatusMessageReceive(msg);
            }
        }else if(typeMSG == MESSAGE){
            Message msg;
            memset(&msg, 0, sizeof(msg));
            recv(newSd, (Message*)&msg, sizeof(msg), 0);
            cout << "Mensagem recebida: " << msg.getMsg() << endl;
        }
        return true;
    }
};
