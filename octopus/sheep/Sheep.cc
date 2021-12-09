#include "Sheep.h"
#include "SheepMobility.h"

#include <iostream>
#include <queue>

using namespace omnetpp;
using namespace std;
using namespace inet;

Define_Module(Sheep);

extern std::queue<UAVMessage> msgs;
extern bool stopped;

int posicao;

void Sheep::initialize(){
    ID = getIndex();
    cout << "INICIOU COMUNICAÇÃO SHEEP!" << endl;
}

void Sheep::handleMessage(cMessage *msg){
    cout << "RECEBEU MENSAGEM SHEEP" << endl;
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);
    cout << "[U2U] Executando ação: " << msg->getFullName() << endl;
    stopped = true;
    delete msg;
}

void Sheep::forwardMessage(UAVMessage *msg){
    cout << "XXXXX" << endl;
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *Sheep::generateMessage(){
    cout << "YYYY" << endl;
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
