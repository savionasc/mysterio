#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../status/StatusC1.h"
#include "../../src/utils/Message.h"


using namespace std;

class SocketReceber {
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){
        //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
        //char msg[1500];
        //memset(&msg, 0, sizeof(msg));
        //recv(newSd, (char*)&msg, sizeof(msg), 0);
        Message msg;
        memset(&msg, 0, sizeof(msg));
        std::cout << "Recebi!" << std::endl;
        recv(newSd, (Message*)&msg, sizeof(msg), 0);
        if(!strcmp(msg.msg, "exit")){
            std::cout << "UAV has quit the session" << std::endl;
            return false;
        }else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
            StatusC1 status;
            Message m;
            status.onMessageReceive(m);
        }
        std::cout << "UAV Message: " << msg.msg << std::endl;
        std::cout << " x " << msg.getCode() << std::endl;
        std::cout << " y " << msg.getDestination() << std::endl;
        std::cout << " z " << msg.getSource() << std::endl;
        return true;
    }
};
