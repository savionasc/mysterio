#include <sys/socket.h>
#include <netinet/in.h>
#include "../../src/utils/Message.h"
#include "../../src/mission/Task.h"
#include "../communication/TaskMessage.h"

class MessageSender {
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
        int codeMessage = MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        send(idUAVSocket, (Message*)&message, sizeof(message), 0);
    }

    bool enviarTarefa(int idUAVSocket, TaskMessage message){
        int codeMessage = TASK_MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        cout << "Task: " << message.code << endl;
        send(idUAVSocket, (TaskMessage*)&message, sizeof(message), 0);
    }
};
