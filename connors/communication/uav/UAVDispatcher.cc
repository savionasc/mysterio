#include "UAVDispatcher.h"

#include <iostream>

#include "../../../src/communication/StatusMessage.h"
#include "UAVMessageReceive.h"
#define HOSTNAME "127.0.0.1"

using namespace std;

namespace mysterio {

void UAVDispatcher::dispatchTaskMessage(TaskMessage msg){
    int codeMessage = Message::TASK_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (TaskMessage*)&msg, sizeof(msg), 0);
}

void UAVDispatcher::dispatchStatusMessage(StatusMessage msg){
    //DroneStatusMessage* s = dynamic_cast<DroneStatusMessage*>(&msg);
    int codeMessage = Message::STATUS_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (StatusMessage*)&msg, sizeof(msg), 0);
}

void UAVDispatcher::dispatchMessage(Message msg){
    //send(this->getSocketCode(), msg.getMsg(), strlen(msg.getMsg()), 0);
    int codeMessage = Message::MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (Message*)&msg, sizeof(msg), 0);
}

void UAVDispatcher::connectBase(){
    if(!this->connected){
        this->connected = true;
        char msg[1500];
        struct hostent* host = gethostbyname(HOSTNAME);
        sockaddr_in sendSockAddr;
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(1111);
        int clientSd = socket(AF_INET, SOCK_STREAM, 0);

        int status = connect(clientSd,
                             (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if(status < 0){
            std::cout<<"Error connecting to socket!"<<std::endl;
            this->setDisconnected();
            //this->connected = false;
        }else{
            std::cout << "Connected to the server!" << std::endl;
            this->setConnected();
            //this->connected = true;
            this->socketCode = clientSd;
            cout << "identificação: " << this->identificacao << endl;
            if(identificacao == 15){
                cout << "LISTONA SIZE: " << this->uavTasks->size() << endl;
            }

            thread receber(UAVMessageReceive(), this->socketCode, this->selfID, this->socketCode, identificacao, this->uavTasks);//, this->identificacao);
            receber.detach();
        }
    }
}

int UAVDispatcher::getSocketCode(){
    return this->socketCode;
}

bool UAVDispatcher::isConnected() {
    return connected;
}

void UAVDispatcher::setConnected() {
    this->connected = true;
}

void UAVDispatcher::setDisconnected() {
    this->connected = false;
}

void UAVDispatcher::setSocketCode(int socketCode) {
    this->socketCode = socketCode;
}

int UAVDispatcher::getSelfID(){
    return this->selfID;
}

void UAVDispatcher::setSelfID(int selfID){
    this->selfID = selfID;
}

void UAVDispatcher::disconnectBase(){
    close(getSocketCode());
}

void UAVDispatcher::reconnect(){
    this->connectBase();
}

}
