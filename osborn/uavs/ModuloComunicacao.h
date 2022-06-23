#ifndef MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_
#define MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_

#include "../../osborn/uavs/UAVMessage_m.h"
#include "inet/common/geometry/common/Coord.h"

using namespace omnetpp;
using namespace std;

namespace inet {

class ModuloComunicacao : public cSimpleModule {
  public:
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
    void rememberCheckMessage(double seconds);

    Coord castCoordinateToCoord(Coordinate co){
        Coord coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    Coordinate castCoordToCoordinate(Coord co){
        Coordinate coor(co.getX(), co.getY(), co.getZ());
        return coor;
    }

    int selfID = -2;
    UAVMessage* sendMSGEvt;
};
}

#endif
