#include "ModuloComunicacao2.h"

#include "../UAVMobility.h"
#include <iostream>
#include <queue>

#include "../../communication/UAVMysCommunication.h"
#include "../../../src/status/UAVStatus.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;

Define_Module(ModuloComunicacao2);

//shared variables
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
extern UAVMysCommunication uavs[NUMUAVS];
extern std::queue<UAVMessage> msgs;

void ModuloComunicacao2::initialize(){
    selfID = getIndex();

    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase();
    }
    //bubble("INICIANDO, Iniciando!");
    cout << "Iniciou comunicação UAV!" << endl;

    UAVMessage *sendMSGEvt = new UAVMessage("checking", 123);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+2, sendMSGEvt);
}

void ModuloComunicacao2::handleMessage(cMessage *msg){
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);

    if(mMSG->getKind() == 123){ //123 lembrar de verificar
        if(msgs.size() > 0){
            UAVMessage umsg = msgs.front();
            if(umsg.getOrigem() == selfID){
                cout << "Mensagem recuperada: " << umsg.getDestino() << "-" << umsg.getOrigem() << "|" << umsg.getFullName() << endl;
                msgs.pop();

                cout << "Enviando mensagem de... " << selfID << endl;
                mMSG->setName("Gostei!");
                //scheduleAt(simTime()+1, &umsg);
                mMSG->setKind(321);
                mMSG->setOrigem(umsg.getOrigem());
                mMSG->setDestino(umsg.getDestino());
                send(mMSG, "out", 0); //0 indicates the first link/door
            }
        }

    }else{
        cout << "RECEBEU MENSAGEM UAV" << endl;
        cout << "[U2U] Executando ação: " << mMSG->getFullName() << endl;
        cout << "[U2U] Origem: " << mMSG->getOrigem() << endl;
        cout << "[U2U] Destino: " << mMSG->getDestino() << endl;
        send(msg, "out", 1); //0 indicates the first link/door
    }

    UAVMessage *checkMSG = new UAVMessage("checking", 123);
    scheduleAt(simTime()+5, checkMSG);
    //delete mMSG;
}

//ATENÇÃO!!
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacao2::forwardMessage(UAVMessage *msg){
    cout << "AAAA" << endl;
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacao2::generateMessage(){
    cout << "BBBB" << endl;
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
void ModuloComunicacao2::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new UAVMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}
