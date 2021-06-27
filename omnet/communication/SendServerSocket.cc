#include <sys/socket.h>
#include <netinet/in.h>
#include "../communication/MysMessage.h"

class SendSocket {
public:
    void operator()(int param, MysMessage param2){
        enviar(param, param2);
    }

    bool enviar(int newSd, MysMessage message){
        send(newSd, (MysMessage*)&message, sizeof(message), 0);
    }
};
