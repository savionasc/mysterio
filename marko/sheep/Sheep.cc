#include "Sheep.h"
#include "SheepMobility.h"

#include <iostream>
#include <queue>

using namespace omnetpp;
using namespace std;
using namespace inet;

Define_Module(Sheep);

extern bool stopped;

int posicao;

void Sheep::initialize(){
    ID = getIndex();
}

void Sheep::handleMessage(cMessage *msg){
    cout << "[SHEEP] Executando ação: " << msg->getFullName() << endl;
    stopped = true;
    delete msg;
}

void Sheep::forwardMessage(UAVMessage *msg){
    int n = gateSize("out");
    int k = intuniform(0, (n-1));
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
