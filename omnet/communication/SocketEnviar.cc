#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class SocketEnviar {
public:
    void operator()(int param){
        enviarResposta(param);
        //while(enviarResposta(param)){ }
    }

    void operator()(int param, char *param2){
        send(param, (char*)param2, strlen(param2), 0);
    }

    bool enviarResposta(int newSd){
        char msg[1500];
        std::cout << ">";
        std::string data;
        cin >> data;
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
        //if(data == "exit")
        send(newSd, (char*)&msg, strlen(msg), 0);
    }
};
