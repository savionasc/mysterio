#ifndef MYSTERIO_OCTOPUS_COMMUNICATION_MESSAGESENDER_H_
#define MYSTERIO_OCTOPUS_COMMUNICATION_MESSAGESENDER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include "../../src/communication/Message.h"
#include "../../src/communication/TaskMessage.h"
#include "../../src/mission/Task.h"
#include "messagestruct.h"

class MessageSender {
public:
    void operator()(int param, Message param2){
        enviar(param, param2);
    }

    void operator()(int param, message *param2){
        enviarmsgstruct(param, *param2);
    }


    void operator()(int param, TaskMessage param2){
        enviarTarefa(param, param2);
    }

    bool enviar(int idUAVSocket, Message message){
        int codeMessage = MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        send(idUAVSocket, (Message*)&message, sizeof(message), 0);
    }

    bool enviarTarefa(int idUAVSocket, TaskMessage message){
        int codeMessage = TASK_MESSAGE;
        send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        send(idUAVSocket, (TaskMessage*)&message, sizeof(message), 0);
    }

    bool enviarmsgstruct(int idUAVSocket, message msg){
        //int codeMessage = TASK_MESSAGE;
        //send(idUAVSocket, (int*)&codeMessage, sizeof(codeMessage), 0);
        //send(idUAVSocket, (message*)&message, sizeof(msg));
        send(idUAVSocket, (message*)&msg, sizeof(msg), 0);
    }
};

#endif
