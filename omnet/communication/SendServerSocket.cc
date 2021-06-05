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
        send(param, (MysMessage*)&param2, sizeof(param2), 0);
    }

    bool enviarResposta(int newSd){
        char msg[1500];
        std::cout << ">";
        std::string data;
        cin >> data;
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
        MysMessage message(msg, 10, -1, newSd); //Nao está enviando pra ninguém...
        send(newSd, (MysMessage*)&message, sizeof(message), 0);
    }
};
