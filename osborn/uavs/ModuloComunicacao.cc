#include "../../osborn/uavs/ModuloComunicacao.h"

#include "../mission/MysTask.h"
#include <iostream>
#include <queue>

#include "../../osborn/mission/MysTask.h"
#include "../../osborn/uavs/UAVMobility.h"
#include "../../src/status/UAVStatus.h"
#include "../../src/communication/ModuleMessage.h"
#include "../communication/uav/UAVMysCommunication.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;

Define_Module(ModuloComunicacao);

//shared variables
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
extern UAVMysCommunication uavs[NUMUAVS];
extern bool ativo[NUMUAVS];
extern std::vector<ModuleMessage> msgs[NUMUAVS];

enum codesUAV{
    CHECKING_MESSAGE = 123,
    REQUEST_POSITION_UAV = 234,
    RESPONSE_POSITION_UAV
};

void ModuloComunicacao::initialize(){
    selfID = getIndex();

    //Mudar as flags e usar assim...
    int x = ModuloComunicacao::RESPONDER_LOCALIZACAO;

    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase();
    }
    cout << "Iniciou comunicação UAV!" << endl;

    //Iniciando procura por novas mensagens
    UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+2, sendMSGEvt);
}

void ModuloComunicacao::handleMessage(cMessage *msg){
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);

    //if(selfID == 1 && mMSG->getOrigem() != 1){
    if(mMSG->getKind() == 234){
        cout << "[XXX] Mensagem recebida em: " << selfID << " de: " << mMSG->getOrigem();
        cout << "Para: " << mMSG->getDestino() << " Tipo: " << mMSG->getKind() << endl;
    }

    //SE MENSAGEM RECEBIDA POR OUTRO UAV
    if(mMSG->getKind() == REQUEST_POSITION_UAV && strcmp(mMSG->getName(), "location") == 0){
        cout << "Mensagem recebida em: " << selfID << " de: " << mMSG->getOrigem();
        cout << " Tipo: " << mMSG->getKind() << endl;

        //RESPONDENDO
        UAVMessage *uavMSG = new UAVMessage("location", RESPONSE_POSITION_UAV);
        uavMSG->setDestino(mMSG->getOrigem());
        uavMSG->setOrigem(selfID);
        uavMSG->setStatus(UAVStatus(castCoordToCoordinate(position[selfID])));
        if(uavMSG->getDestino() > selfID){
            send(uavMSG, "out", uavMSG->getDestino()-1);
        }else{
            send(uavMSG, "out", uavMSG->getDestino());
        }
    }else if(mMSG->getKind() == RESPONSE_POSITION_UAV){
        UAVStatus us = mMSG->getStatus();
        cout << "[UAV"<< mMSG->getOrigem() <<"-STATUS-TO-U"<<selfID<<"] Status recebido: x:" << us.getLocationX();
        cout << " y: " << us.getLocationY();
        cout << " z: " << us.getLocationZ() << endl;
        ModuleMessage mm = castUAVMessageToModuleMessage(*mMSG);
        mm.setModule(2);

        msgs[selfID].push_back(mm);
    }

    //SE MENSAGEM FOR CHECKING OU HOUVER MENSAGEM VINDA DO MOBILITY
    if(mMSG->getKind() == CHECKING_MESSAGE && strcmp(mMSG->getName(), "checking") == 0){

        for (int i = 0; i < msgs[selfID].size() > 0; i++) {
            ModuleMessage mm = msgs[selfID][i];
            if(mm.getModule() == MODULE_ID){
                cout << "[MxM] " << mm.getMsg() << " | " << mm.getSource() << endl;
                if(strcmp(mm.getMsg(), "location") == 0 && mm.getCode() == REQUEST_POSITION_UAV){
                    cout << "[MM] ESTÁ QUERENDO SABER LOCATION: " << mm.getSource() << endl;
                    UAVMessage *uavMSG = new UAVMessage(mm.getMsg(), mm.getCode());
                    uavMSG->setOrigem(selfID);
                    //uavMSG->setDestino(i+1);
                    //send(uavMSG, "out", uavMSG->getDestino()-1);

                    enviarMensagemParaTodosOsUAVs(uavMSG);
                }

                msgs[selfID].pop_back();
            }

        }

        UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
        sendMSGEvt->setOrigem(selfID);
        scheduleAt(simTime()+2, sendMSGEvt);
    }

    delete mMSG;
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

void ModuloComunicacao::enviarMensagemParaTodosOsUAVs(UAVMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");

    int id = 0;
    for (int i = 0; i < n; i++) {

        if(selfID == id){
            id += 1;
        }

        msg->setDestino(id);
        send(msg->dup(), "out", i);
        id++;
    }

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
