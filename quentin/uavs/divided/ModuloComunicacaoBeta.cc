#include "../../../quentin/uavs/divided/ModuloComunicacaoBeta.h"

#include <iostream>

#include "../../../src/status/UAVStatus.h"

using namespace omnetpp;
using namespace inet;

Define_Module(ModuloComunicacaoBeta);

void ModuloComunicacaoBeta::initialize(){
    selfID = getIndex();

    sendMSGEvt = new UAVMessage("Opa", 0);
    sendMSGEvt->setDestino(selfID);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+1, sendMSGEvt);
}

void ModuloComunicacaoBeta::handleMessage(cMessage *msg){
}

void ModuloComunicacaoBeta::forwardMessage(UAVMessage *msg){
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacaoBeta::generateMessage(){
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

void ModuloComunicacaoBeta::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new UAVMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}
