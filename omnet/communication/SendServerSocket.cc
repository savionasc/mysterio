#include <sys/socket.h>
#include <netinet/in.h>
#include "../../src/utils/Message.h"
#include "../communication/TaskMessage.h"

class SendServerSocket {
public:
    void operator()(int param, Message param2){
        enviar(param, param2);
        cout << "Message Enviada" << endl;
    }

    void operator()(int param, TaskMessage param2){
        enviarTarefa(param, param2);
        cout << "TaskMessage Enviada" << endl;
    }

    bool enviar(int idUAVSocket, Message message){
        send(idUAVSocket, (Message*)&message, sizeof(message), 0);
    }

    bool enviarTarefa(int idUAVSocket, TaskMessage message){
        send(idUAVSocket, (TaskMessage*)&message, sizeof(message), 0);
    }
};
