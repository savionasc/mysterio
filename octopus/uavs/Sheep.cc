#include "Sheep.h"

#include <iostream>

using namespace omnetpp;
using namespace std;
using namespace inet;

Define_Module(Sheep);

int posicao;

void Sheep::initialize(){
    ID = getIndex();
}

void Sheep::handleMessage(UAVMessage *msg){
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);
    cout << "[U2U] Executando ação: " << msg->getFullName() << endl;
}

void Sheep::forwardMessage(UAVMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *Sheep::generateMessage(){
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "msg-%d-para-%d", src, dest);

    UAVMessage *msg = new UAVMessage(msgname);
    msg->setOrigem(src);
    msg->setDestino(dest);

    return msg;
}
