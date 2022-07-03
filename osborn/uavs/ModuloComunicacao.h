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
extern int itera[NUMUAVS];
extern std::vector<ModuleMessage> msgs[NUMUAVS];
extern std::vector<Task> tasksVector[NUMUAVS];

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

    //Auxiliary functions
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    void enviarMensagemParaTodosOsUAVs(UAVMessage *msg, int size);
    void enviarMensagemParaTodosOsUAVsAtivos(UAVMessage *msg);
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

    Coord pegarPosicaoUAV(){
        //char uavChar[] = "uav["+this->selfID+"]";
        //cModule *uav = getParentModule()->getParentModule()->getSubmodule("uav[0]")->getSubmodule("mobility", selfID);
        //cModule *uav = getParentModule()->getSubmodule("UAV[1]");//->getSubmodule("mobility", 0);//->getSubmodule("uav[1]");//->getSubmodule("mobility", 0);
        char buf[20];
        sprintf(buf, "UAV[%d]", selfID);
        cModule * mod = getModuleByPath(buf);
        cModule *uav = mod->getSubmodule("mobility", 0);
        UAVMobility *uavmob = check_and_cast<UAVMobility*>(uav);

       /*for (cModule::SubmoduleIterator it(getParentModule()); !it.end(); ++it) {
           cModule *submodule = *it;
           cout << "1MODULO: " << submodule << endl;
       }*/

        cout << "MODULO: " << uavmob->posicaoAtual() << endl;
        return uavmob->posicaoAtual();
    }

    int selfID = -2;
    UAVMessage* sendMSGEvt;
    int qtdFormacao = 0;
};
}

#endif
