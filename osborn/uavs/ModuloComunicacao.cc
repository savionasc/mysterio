#include "../../osborn/uavs/ModuloComunicacao.h"

#include "../mission/MysTask.h"
#include <iostream>
#include <queue>

#include "../../osborn/mission/MysTask.h"
#include "../../osborn/uavs/UAVMobility.h"
#include "../../src/status/UAVStatus.h"
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
extern std::queue<TaskMessage> msgs;

enum codesUAV{
    CHECKING_MESSAGE = 123,
    SHEEP_ALERT,
    SUBTASK_HIGH_PRIORITY = 222
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

    //SE MENSAGEM RECEBIDA POR OUTRO UAV
    if(mMSG->getKind() == 234 && strcmp(mMSG->getName(), "location") == 0){
        cout << "IF" << endl;
        if(selfID != 0){
            cout << "ELSE != 0 || 1" << endl;
            cout << "Mensagem recebida em: " << selfID << " de: " << mMSG->getDestino();
            cout << " Tipo: " << mMSG->getKind() << endl;

            //RESPONDENDO
            UAVMessage *uavMSG = new UAVMessage("location", 235);
            uavMSG->setOrigem(selfID);
            uavMSG->setDestino(mMSG->getOrigem());
            uavMSG->setStatus(UAVStatus(castCoordToCoordinate(position[selfID])));
            send(uavMSG, "out", uavMSG->getDestino());
        }
    }else if(mMSG->getKind() == 235){
        UAVStatus us = mMSG->getStatus();
        cout << "[UAV"<< mMSG->getOrigem() <<"-STATUS] Status recebido: x:" << us.getLocationX();
        cout << " y: " << us.getLocationY();
        cout << " z: " << us.getLocationZ() << endl;
    }

    //SE MENSAGEM FOR CHECKING OU HOUVER MENSAGEM VINDA DO MOBILITY
    if(mMSG->getKind() == CHECKING_MESSAGE && strcmp(mMSG->getName(), "checking") == 0){
        if(msgs.size() > 0 && selfID == msgs.front().getSource()){
            TaskMessage tm = msgs.front();
            cout << "[MxM] " << tm.getMsg() << " | " << tm.getSource() << endl;
            if(strcmp(tm.getMsg(), "location") == 0 && tm.getCode() == 234){
                cout << "[MM] ESTÁ QUERENDO SABER LOCATION: " << tm.getSource() << endl;
                cout << "IF" << endl;
                if(selfID == 0){
                    cout << "IF == 0" << endl;
                    for (int i = 0; i < 2; i++) {
                        cout << "FOR" << endl;
                        //UAVMessage *uavMSG = new UAVMessage("ToU", SUBTASK_HIGH_PRIORITY);
                        UAVMessage *uavMSG = new UAVMessage(tm.getMsg(), tm.getCode());
                        uavMSG->setOrigem(selfID);
                        uavMSG->setDestino(i+1);
                        send(uavMSG, "out", uavMSG->getDestino()-1);

                        cout << "Mensagem enviada para: " << i+1 << endl;
                    }
                }else{
                    cout << "ELSE != 0 || 2" << endl;
                    cout << "Mensagem recebida em: " << selfID << " de: " << mMSG->getOrigem();
                    cout << " Tipo: " << mMSG->getKind() << endl;
                }
            }

            /*if(tm.getDestination() == -5){ //Sheep
                if(selfID == 0)
                    cout << "Terceiro if" << endl;
                UAVMessage *sheepAlert = new UAVMessage("STOPSHEEP", SHEEP_ALERT);
                sheepAlert->setOrigem(selfID);
                send(sheepAlert, "out", 0);
            }else{ //Others UAVs
                if(selfID == 0)
                    cout << "Else no if" << endl;
                UAVMessage *uavMSG = new UAVMessage("ToU", SUBTASK_HIGH_PRIORITY);
                uavMSG->setOrigem(selfID);
                uavMSG->setDestino(tm.getDestination());
                uavMSG->setTask(tm.getTask());
                send(uavMSG, "out", uavMSG->getDestino());
            }*/

            msgs.pop();
        }

        UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
        sendMSGEvt->setOrigem(selfID);
        scheduleAt(simTime()+2, sendMSGEvt);
    }/*else if(mMSG->getKind() == SUBTASK_HIGH_PRIORITY && mMSG->getDestino() == selfID){
        ativo[selfID] = true;

        Task x = mMSG->getTask();
        int i = x.getUAV().getID();
        tasksVector[i].push_back(x);
        int j = tasksVector[i].size()-1;
        tasksVector[i][j].setType(x.getType());
        tasksVector[i][j].getUAV().setID(x.getUAV().getID());
        waypoints[i] = tasksVector[i][j].getWaypoints();
        if(itera[i] < 0){
            itera[i]++;
        }
    }*/

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
