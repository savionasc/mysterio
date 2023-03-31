#include "../../../quentin/uavs/divided/ModuloComunicacaoAlpha.h"

#include <iostream>

#include "../../../src/status/UAVStatus.h"

using namespace omnetpp;
using namespace inet;

Define_Module(ModuloComunicacaoAlpha);

void ModuloComunicacaoAlpha::initialize(){
    selfID = getIndex();
    sendMSGEvt = new UAVMessage("Opa", 0);
    sendMSGEvt->setDestino(selfID);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+1, sendMSGEvt);
}

void ModuloComunicacaoAlpha::handleMessage(cMessage *msg){
}

void ModuloComunicacaoAlpha::forwardMessage(UAVMessage *msg){
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacaoAlpha::generateMessage(){
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

void ModuloComunicacaoAlpha::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new UAVMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}
