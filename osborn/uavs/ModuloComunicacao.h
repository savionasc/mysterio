#ifndef MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_
#define MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_

#include "../../osborn/uavs/UAVMessage_m.h"
#include "../../src/communication/ModuleMessage.h"
#include "inet/common/geometry/common/Coord.h"

using namespace omnetpp;
using namespace std;

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
    void enviarMensagemParaTodosOsUAVs(UAVMessage *msg);
    void rememberCheckMessage(double seconds);

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

        return mMessage;
    }

    UAVMessage castModuleMessageToUAVMessage(ModuleMessage moduleMessage){
        UAVMessage uMessage(strdup(moduleMessage.getMsg()), moduleMessage.getCode());
        uMessage.setOrigem(moduleMessage.getSource());
        uMessage.setDestino(moduleMessage.getDestination());
        uMessage.setStatus(moduleMessage.getStatus());
        uMessage.setTask(moduleMessage.getTask());

        return uMessage;
    }

    int selfID = -2;
    UAVMessage* sendMSGEvt;
};
}

#endif
