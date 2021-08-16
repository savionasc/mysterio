#include <sys/socket.h>
#include <netinet/in.h>
#include "../../src/utils/Message.h"
#include "../communication/DroneStatusMessage.h"

class SendServerSocket {
public:
    void operator()(int param, Message param2){
        enviar(param, param2);
    }

    bool enviar(int idUAVSocket, Message message){
        send(idUAVSocket, (Message*)&message, sizeof(message), 0);
    }
};
