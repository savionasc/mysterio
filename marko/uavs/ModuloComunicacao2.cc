#include "ModuloComunicacao2.h"

#include "../mission/MysTask.h"
#include "UAVMobility.h"
#include <iostream>
#include <queue>

#include "../communication/UAVMysCommunication.h"
#include "../../src/status/UAVStatus.h"

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
extern bool ativo[NUMUAVS];
extern std::queue<TaskMessage> msgs;

enum codesUAV{
    CHECKING_MESSAGE = 123,
    SHEEP_ALERT,
    SUBTASK_HIGH_PRIORITY = 222
};

void ModuloComunicacao2::initialize(){
    selfID = getIndex();

    int x = ModuloComunicacao2::RESPONDER_LOCALIZACAO;

    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase();
    }
    cout << "Iniciou comunicação UAV!" << endl;
    UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+2, sendMSGEvt);
}

void ModuloComunicacao2::handleMessage(cMessage *msg){
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);
    if(mMSG->getKind() == CHECKING_MESSAGE && strcmp(mMSG->getName(), "checking") == 0){
        if(msgs.size() > 0 && selfID == msgs.front().getSource()){
            TaskMessage tm = msgs.front();
            if(tm.getDestination() == -5){ //Sheep
                UAVMessage *sheepAlert = new UAVMessage("STOPSHEEP", SHEEP_ALERT);
                sheepAlert->setOrigem(selfID);
                send(sheepAlert, "out", 0);
            }else{ //Others UAVs
                UAVMessage *uavMSG = new UAVMessage("ToU", SUBTASK_HIGH_PRIORITY);
                uavMSG->setOrigem(selfID);
                uavMSG->setDestino(tm.getDestination());
                uavMSG->setTask(tm.getTask());
                send(uavMSG, "out", uavMSG->getDestino());
            }
            msgs.pop();
        }

        UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
        sendMSGEvt->setOrigem(selfID);
        scheduleAt(simTime()+2, sendMSGEvt);
    }else if(mMSG->getKind() == SUBTASK_HIGH_PRIORITY && mMSG->getDestino() == selfID){
        ativo[selfID] = true;

        Task x = mMSG->getTask();
        int i = x.getUAV().getID();
        base[i].push_back(x);
        int j = base[i].size()-1;
        base[i][j].setType(x.getType());
        base[i][j].getUAV().setID(x.getUAV().getID());
        waypoints[i] = base[i][j].getWaypoints();
        cout << "TT: " << base[i][j].getUAV().getID() << endl;
        if(itera[i] < 0){
            itera[i]++;
        }
    }

    delete mMSG;
}

//ATENÇÃO!!
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacao2::forwardMessage(UAVMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacao2::generateMessage(){
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
