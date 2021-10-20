#include <iostream>

#include "UAVMysCommunication.h"
#include "UAVMessageReceive.h"
#include "../../src/communication/StatusMessage.h"
#define HOSTNAME "127.0.0.1"

using namespace std;

namespace mysterio {

void UAVMysCommunication::dispatchTaskMessage(TaskMessage msg){
    int codeMessage = TASK_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (TaskMessage*)&msg, sizeof(msg), 0);
}

void UAVMysCommunication::dispatchStatusMessage(StatusMessage msg){
    //DroneStatusMessage* s = dynamic_cast<DroneStatusMessage*>(&msg);
    int codeMessage = STATUS_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (StatusMessage*)&msg, sizeof(msg), 0);
}

void UAVMysCommunication::dispatchMessage(Message msg){
    //send(this->getSocketCode(), msg.getMsg(), strlen(msg.getMsg()), 0);
    int codeMessage = MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (Message*)&msg, sizeof(msg), 0);
}

void UAVMysCommunication::connectBase(){
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
            this->connected = false;
        }else{
            std::cout << "Connected to the server!" << std::endl;
            this->connected = true;
            this->socketCode = clientSd;

            thread receber(UAVMessageReceive(), this->socketCode, this->selfID, this->socketCode);
            receber.detach();
        }
    }
}

int UAVMysCommunication::getSocketCode(){
    return this->socketCode;
}

bool UAVMysCommunication::isConnected() {
    return connected;
}

void UAVMysCommunication::setConnected(bool connected) {
    this->connected = connected;
}

void UAVMysCommunication::setSocketCode(int socketCode) {
    this->socketCode = socketCode;
}

int UAVMysCommunication::getSelfID(){
    return this->selfID;
}

void UAVMysCommunication::setSelfID(int selfID){
    this->selfID = selfID;
}

void UAVMysCommunication::disconnectBase(){
    close(getSocketCode());
}

void UAVMysCommunication::reconnect(){
    this->connectBase();
}

}
