#include "UAVCommunicationSocket.h"
#include "DroneStatusMessage.h"
#include <iostream>

#include "SocketMessageReceive.h"
#define HOSTNAME "127.0.0.1"

using namespace std;

namespace mysterio {

void UAVCommunicationSocket::dispatchTaskMessage(TaskMessage msg){
    int codeMessage = TASK_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (TaskMessage*)&msg, sizeof(msg), 0);
}

void UAVCommunicationSocket::dispatchStatusMessage(DroneStatusMessage msg){
    //DroneStatusMessage* s = dynamic_cast<DroneStatusMessage*>(&msg);
    int codeMessage = STATUS_MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (DroneStatusMessage*)&msg, sizeof(msg), 0);
}

void UAVCommunicationSocket::dispatchMessage(Message msg){
    //Aqui envia mensagem via socket pro Communication
    //send(this->getSocketCode(), msg.getMsg(), strlen(msg.getMsg()), 0);
    int codeMessage = MESSAGE;
    send(this->getSocketCode(), (int*)&codeMessage, sizeof(codeMessage), 0);
    send(this->getSocketCode(), (Message*)&msg, sizeof(msg), 0);
}

void UAVCommunicationSocket::connectBase(){
    if(!this->connected){
        this->connected = true;
        char msg[1500];
        struct hostent* host = gethostbyname(HOSTNAME);
        sockaddr_in sendSockAddr;
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(PORT);
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

            thread receber(SocketMessageReceive(), this->socketCode, this->selfID, this->socketCode);
            receber.detach(); //join
        }

        //void CommunicationSocket::connectANewUAV(int ID, StatusC1 *aggregator){
        //    UAV newUAV;
        //    newUAV.setID(ID);
        //    aggregator->addUAV(newUAV);
        //}
    }
}

int UAVCommunicationSocket::getSocketCode(){
    return this->socketCode;
}

bool UAVCommunicationSocket::isConnected() {
    return connected;
}

void UAVCommunicationSocket::setConnected(bool connected) {
    this->connected = connected;
}

void UAVCommunicationSocket::setSocketCode(int socketCode) {
    this->socketCode = socketCode;
}

int UAVCommunicationSocket::getSelfID(){
    return this->selfID;
}

void UAVCommunicationSocket::setSelfID(int selfID){
    this->selfID = selfID;
}

void UAVCommunicationSocket::disconnectBase(){
    close(getSocketCode());
}

}
