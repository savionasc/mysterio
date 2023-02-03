#include "ModuloComunicacao.h"

#include "../../mission/GoTo.h"
#include "../UAVMobility.h"
#include <iostream>

#include "../../../src/status/UAVStatus.h"
#include "../../communication/uav/UAVDispatcher.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;

Define_Module(ModuloComunicacao);

//shared variables
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];

void ModuloComunicacao::initialize(){
    selfID = getIndex();
    mobility = getMobility();

    bubble("INICIANDO, Iniciando!");
    sendMSGEvt = new UAVMessage("Opa", 0);
    sendMSGEvt->setDestino(selfID);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+1, sendMSGEvt);
}

void ModuloComunicacao::handleMessage(cMessage *msg){
    ///AQUIIIIII
    /*int x = par("savio");
    int random = 1+(rand() % 100);
    cout << "Savio: " << random << endl;
    par("savio").setIntValue(random);
    sendMSGEvt = new UAVMessage("Opa", 0);
    sendMSGEvt->setDestino(selfID);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+3, sendMSGEvt);*/
}

//ATENÇÃO!!
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacao::forwardMessage(UAVMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacao::generateMessage(){
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

//Para usar com a mensagem //sprintf(msgname, "msg-%d-para-%d", src, dest);

//Auxiliary functions
void ModuloComunicacao::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new UAVMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}

UAVMobility* ModuloComunicacao::getMobility(){
    cSimulation *currentSimulation = getSimulation();
    UAVMobility *mob = nullptr;
    // UAVMobility of UAV0 is accessible by "MysterioCommunication.UAV[0].mobility"
    // ModuloComunicacao of UAV0 is accessible by "MysterioCommunication.UAV[0]"
    string str = "MysterioCommunication.UAV["+to_string(getIndex())+"].mobility";
    char path[str.length()+1];
    strcpy(path, str.c_str());
    //mob = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath());
    mob = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath(path));

    return mob;
}
