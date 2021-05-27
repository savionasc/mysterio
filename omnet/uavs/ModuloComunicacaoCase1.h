#ifndef MYSTERIO_OMNET_EXCOMM1_UAVS_MODULOCOMUNICACAOCASE1_H_
#define MYSTERIO_OMNET_EXCOMM1_UAVS_MODULOCOMUNICACAOCASE1_H_

#include "../common/MinhaMensagem_m.h"

using namespace omnetpp;
using namespace std;

namespace inet {

class ModuloComunicacaoCase1 : public cSimpleModule {
  public:
    enum TipoMensagem {
        SOLICITAR_LOCALIZACAO = 10,
        RESPONDER_LOCALIZACAO,
        SOLICITAR_VELOCIDADE,
        RESPONDER_VELOCIDADE
    };
  protected:
    virtual MinhaMensagem *generateMessage();
    virtual void forwardMessage(MinhaMensagem *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Auxiliary functions
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    void rememberCheckMessage(double seconds);
    void solicitarStatusDoUAVVizinho();

    int selfID = -2;
    MinhaMensagem* sendMSGEvt;
};
}

#endif
