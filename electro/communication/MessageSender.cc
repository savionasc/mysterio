#ifndef MYSTERIO_ELECTRO_COMMUNICATION_MESSAGESENDER_H_
#define MYSTERIO_ELECTRO_COMMUNICATION_MESSAGESENDER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include "../../src/communication/Message.h"
#include "../../src/communication/TaskMessage.h"
#include "../../src/mission/Task.h"

class MessageSender {
public:
    void operator()(int param, Message param2){
        enviar(param, param2);
    }

    void operator()(int param, TaskMessage param2){
        enviarTarefa(param, param2);
    }

    bool enviar(int idUAVSocket, Message message){
        int codeMessage = Message::MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        send(idUAVSocket, (Message*)&message, sizeof(message), 0);
    }

    bool enviarTarefa(int idUAVSocket, TaskMessage message){
        int codeMessage = Message::TASK_MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        send(idUAVSocket, (TaskMessage*)&message, sizeof(message), 0);
    }
};

#endif
