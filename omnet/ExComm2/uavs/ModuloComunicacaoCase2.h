#ifndef MYSTERIO_SAMPLES_EXCOMM2_MODULOCOMUNICACAOCASE2_H_
#define MYSTERIO_SAMPLES_EXCOMM2_MODULOCOMUNICACAOCASE2_H_

#include "../../common/msg/MinhaMensagem_m.h"

using namespace omnetpp;
using namespace std;

namespace inet {

class ModuloComunicacaoCase2 : public cSimpleModule {
  public:
    ModuloComunicacaoCase2();
    virtual ~ModuloComunicacaoCase2();
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
    int selfID = -2;
    MinhaMensagem* sendMSGEvt;
    void rememberCheckMessage(double seconds);
    //void rememberCheckMessage(double seconds, MinhaMensagem *m);
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
};
}/* namespace inet */

#endif /* MYSTERIO_SRC_MODULOCOMUNICACAOCASE2_H_ */
