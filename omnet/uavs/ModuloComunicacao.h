#ifndef MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_
#define MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_

#include "../common/DroneMessage_m.h"
#include "inet/power/base/EpEnergyStorageBase.h"

using namespace omnetpp;
using namespace std;


namespace inet {
using namespace power;

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
        RESPONDER_TEMPOVOO
    };
  protected:
    virtual DroneMessage *generateMessage();
    virtual void forwardMessage(DroneMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Auxiliary functions
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    void rememberCheckMessage(double seconds);
    void solicitarStatusDoUAVVizinho();
    J pegarBateria(int idUAV);
    void atualizarDados();

    int selfID = -2;
    DroneMessage* sendMSGEvt;
};
}

#endif
