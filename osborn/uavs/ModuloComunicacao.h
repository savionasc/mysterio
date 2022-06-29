#ifndef MYSTERIO_OSBORN_UAVS_MODULOCOMUNICACAO_H_
#define MYSTERIO_OSBORN_UAVS_MODULOCOMUNICACAO_H_

#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/communication/ModuleMessage.h"
#include "inet/common/geometry/common/Coord.h"

#include <iostream>
#include <queue>

#include "../mission/MysTask.h"
#include "../../osborn/mission/MysTask.h"
#include "../../osborn/uavs/UAVMobility.h"
#include "../../src/status/UAVStatus.h"
#include "../../src/communication/ModuleMessage.h"
#include "../communication/uav/UAVMysCommunication.h"


using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace mysterio;

//shared variables
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
extern UAVMysCommunication uavs[NUMUAVS];
extern bool ativo[NUMUAVS];
extern std::vector<ModuleMessage> msgs[NUMUAVS];

namespace inet {

class ModuloComunicacao : public cSimpleModule {
  public:
    const int MODULE_ID = 1;
    enum TipoMensagem {
        SOLICITAR_LOCALIZACAO = 10,
        RESPONDER_LOCALIZACAO,
        SOLICITAR_VELOCIDADE,
        RESPONDER_VELOCIDADE,
        SOLICITAR_BATERIA,
        RESPONDER_BATERIA,
        SOLICITAR_TEMPOVOO,
        RESPONDER_TEMPOVOO,
        GOTOTASK,
        TASKCOMPLETED
    };
    static TipoMensagem tipo;
  protected:
    virtual UAVMessage *generateMessage();
    virtual void forwardMessage(UAVMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    /*virtual void handleMessage(cMessage *msg) override
    {
        UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);
        //SE MENSAGEM RECEBIDA POR OUTRO UAV
        if (mMSG->getKind() == REQUEST_POSITION_UAV
                && strcmp(mMSG->getName(), "location") == 0) {
            cout << "Mensagem recebida em: " << selfID << " de: "
                    << mMSG->getOrigem();
            cout << " Tipo: " << mMSG->getKind() << endl;
            //RESPONDENDO
            UAVMessage *uavMSG = new UAVMessage("location",
                    RESPONSE_POSITION_UAV);
            uavMSG->setDestino(mMSG->getOrigem());
            uavMSG->setOrigem(selfID);
            uavMSG->setStatus(
                    UAVStatus(castCoordToCoordinate(position[selfID])));
            if (uavMSG->getDestino() > selfID) {
                send(uavMSG, "out", uavMSG->getDestino() - 1);
            } else {
                send(uavMSG, "out", uavMSG->getDestino());
            }
            UAVStatus us = uavMSG->getStatus();
            cout << "[REQUEST][UAV" << mMSG->getOrigem() << "-STATUS-TO-U"
                    << selfID << "] Status recebido: x:" << us.getLocationX();
            cout << " y: " << us.getLocationY();
            cout << " z: " << us.getLocationZ() << endl;
        } else if (mMSG->getKind() == RESPONSE_POSITION_UAV) {
            UAVStatus us = mMSG->getStatus();
            cout << "[RESPONSE][UAV" << mMSG->getOrigem() << "-STATUS-TO-U"
                    << selfID << "] Status recebido: x:" << us.getLocationX();
            cout << " y: " << us.getLocationY();
            cout << " z: " << us.getLocationZ() << endl;
            ModuleMessage mm = castUAVMessageToModuleMessage(*mMSG);
            mm.setModule(2);
            msgs[selfID].push_back(mm);
        } else if (mMSG->getKind() == REQUEST_CONSENSUS
                && strcmp(mMSG->getName(), "collision") == 0) {
            cout << "minha coordenadas: " << position[mMSG->getOrigem()].getX()
                    << "|" << position[mMSG->getOrigem()].getY() << "|"
                    << position[mMSG->getOrigem()].getZ() << endl;
            cout << "SAVIOOOO" << endl;
            cout << "UAV" << mMSG->getOrigem() << " Quer: " << mMSG->getKind();
            cout << " nome: " << mMSG->getName() << " de: " << selfID
                    << " mesmo que " << mMSG->getDestino() << endl;
            Collision c = mMSG->getCollision();
            cout << "Collision- UAV" << c.getUAV().getID() << " escapar por: "
                    << c.getUAVCase() << endl;
            cout << "Nas coordenadas: " << c.getCoordinate().getX() << "|"
                    << c.getCoordinate().getY() << "|"
                    << c.getCoordinate().getZ() << endl;
            cout << "Coordenadas reais: " << position[selfID].getX() << "|"
                    << position[selfID].getY() << "|" << position[selfID].getZ()
                    << endl;
            ModuleMessage mm = castUAVMessageToModuleMessage(*mMSG);
            mm.setModule(2);
            msgs[selfID].push_back(mm);
        }

        //SE MENSAGEM FOR CHECKING OU HOUVER MENSAGEM VINDA DO MOBILITY
        if (mMSG->getKind() == CHECKING_MESSAGE
                && strcmp(mMSG->getName(), "checking") == 0) {
            for (int i = 0; i < msgs[selfID].size(); i++) {
                ModuleMessage mm = msgs[selfID][i];
                if (mm.getModule() == MODULE_ID) {
                    cout << "[MxM] " << mm.getMsg() << " | " << mm.getSource()
                            << endl;
                    if (strcmp(mm.getMsg(), "location") == 0
                            && mm.getCode() == REQUEST_POSITION_UAV) {
                        cout << "[MM] ESTÁ QUERENDO SABER LOCATION: "
                                << mm.getSource() << endl;
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(),
                                mm.getCode());
                        uavMSG->setOrigem(selfID);
                        //uavMSG->setDestino(i+1);
                        //send(uavMSG, "out", uavMSG->getDestino()-1);
                        enviarMensagemParaTodosOsUAVs(uavMSG);
                    } else if (strcmp(mm.getMsg(), "collision") == 0
                            && mm.getCode() == REQUEST_CONSENSUS) {
                        cout << "[MM] ESTÁ QUERENDO SABER CONSENSUS: "
                                << mm.getSource() << endl;
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(),
                                mm.getCode());
                        uavMSG->setOrigem(selfID);
                        uavMSG->setDestino(mm.getDestination());
                        uavMSG->setCollision(mm.getCollision());
                        if (uavMSG->getDestino() > selfID) {
                            send(uavMSG, "out", uavMSG->getDestino() - 1);
                        } else {
                            send(uavMSG, "out", uavMSG->getDestino());
                        }
                    }

                    auto it = msgs[selfID].begin();
                    it = it + i;
                    //msgs[selfID].pop_back();
                    msgs[selfID].erase(it);
                }
            }
            UAVMessage *sendMSGEvt = new UAVMessage("checking",
                    CHECKING_MESSAGE);
            sendMSGEvt->setOrigem(selfID);
            scheduleAt(simTime() + 2, sendMSGEvt);
        }
        delete mMSG;
    }*/

    //Auxiliary functions
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    void enviarMensagemParaTodosOsUAVs(UAVMessage *msg);
    void rememberCheckMessage(double seconds);
    void handleNessagesBetweenModules(UAVMessage *mMSG);
    void handleNessagesBetweenUAVs(UAVMessage *mMSG);

    Coord castCoordinateToCoord(Coordinate co){
        Coord coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    Coordinate castCoordToCoordinate(Coord co){
        Coordinate coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    ModuleMessage castUAVMessageToModuleMessage(UAVMessage uavMessage){
        ModuleMessage mMessage(strdup(uavMessage.getName()), uavMessage.getKind());
        mMessage.setDestination(uavMessage.getDestino());
        mMessage.setSource(uavMessage.getOrigem());
        mMessage.setStatus(uavMessage.getStatus());
        mMessage.setTask(uavMessage.getTask());
        mMessage.setCollision(uavMessage.getCollision());
        return mMessage;
    }

    UAVMessage castModuleMessageToUAVMessage(ModuleMessage moduleMessage){
        UAVMessage uMessage(strdup(moduleMessage.getMsg()), moduleMessage.getCode());
        uMessage.setOrigem(moduleMessage.getSource());
        uMessage.setDestino(moduleMessage.getDestination());
        uMessage.setStatus(moduleMessage.getStatus());
        uMessage.setTask(moduleMessage.getTask());
        uMessage.setCollision(moduleMessage.getCollision());

        return uMessage;
    }

    int getUAVsAtivos(){
        int qtd = 0;
        for (int j = 0; j < NUMUAVS; j++) {
            if(ativo[j])
                qtd++;
        }
        return qtd;
    }

    int selfID = -2;
    UAVMessage* sendMSGEvt;
};
}

#endif
