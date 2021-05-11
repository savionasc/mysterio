#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../status/StatusC1.h"
#include "../../src/utils/Message.h"


using namespace std;

class ReceiveSocket{
public:
    void operator()(int param){
        while(esperarMensagem(param)){ }
    }

    bool esperarMensagem(int newSd){
        //Aqui deve converter toda e qualquer mensagem e repassar pra this.OnMessageReceve
        Message msg;
        memset(&msg, 0, sizeof(msg));
        recv(newSd, (Message*)&msg, sizeof(msg), 0);
        if(!strcmp(msg.msg, "exit")){
            std::cout << "UAV has quit the session" << std::endl;
            return false;
        }//else if(!strcmp(msg.msg, "status")){ //Mudar isso aqui e chamar o OnMessageReceve
        else if(msg.code > 10 && msg.code < 21){
            StatusC1 status;
            status.onMessageReceive(msg);
        }else {
            StatusC1 status;
            status.onMessageReceive(msg);
        }
        std::cout << "UAV Message: " << msg.msg << std::endl;
        return true;
    }
};
