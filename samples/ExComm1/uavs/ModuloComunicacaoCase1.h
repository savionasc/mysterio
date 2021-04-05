#ifndef MYSTERIO_SAMPLES_EXCOMM1_UAVS_MODULOCOMUNICACAOCASE1_H_
#define MYSTERIO_SAMPLES_EXCOMM1_UAVS_MODULOCOMUNICACAOCASE1_H_

#include "../../common/msg/MinhaMensagem_m.h"

using namespace omnetpp;
using namespace std;

namespace inet {

class ModuloComunicacaoCase1 : public cSimpleModule {
  public:
    ModuloComunicacaoCase1();
    virtual ~ModuloComunicacaoCase1();
    enum TipoMensagem {
        LEMBRAR = 10,
        SOLICITAR_LOCALIZACAO,
        RESPONDER_LOCALIZACAO,
        SOLICITAR_VELOCIDADE,
        RESPONDER_VELOCIDADE
    };
  protected:
    virtual MinhaMensagem *generateMessage();
    virtual void forwardMessage(MinhaMensagem *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    int selfID = -2;
    MinhaMensagem* sendMSGEvt;
    void rememberCheckMessage(double seconds);
};
}/* namespace inet */

#endif /* MYSTERIO_SRC_MODULOCOMUNICACAOCASE1_H_ */
