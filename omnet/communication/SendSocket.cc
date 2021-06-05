#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../common/MysMessage.h"

using namespace std;

class SendSocket {
public:
    void operator()(int param){
        enviarResposta(param);
    }

    void operator()(int param, MysMessage param2){
    //void operator()(int param, char *param2){
        //send(param, (char*)param2, strlen(param2), 0);
        send(param, (MysMessage*)&param2, sizeof(param2), 0);
    }

    bool enviarResposta(int newSd){
        char msg[1500];
        std::cout << ">";
        std::string data;
        cin >> data;
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
        //if(data == "exit")
        //Message *message = new Message(msg, 10, -1, newSd);
        MysMessage message;
        //Message *msg = new Message(m, 10, -1, i);
        message.setCode(10);
        message.setMsg(msg);
        message.setDestination(newSd);
        message.setSource(-1);
        send(newSd, (MysMessage*)&message, sizeof(message), 0);
        //send(newSd, (char*)&msg, strlen(msg), 0);
    }
};
