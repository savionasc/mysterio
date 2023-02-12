#ifndef MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_
#define MYSTERIO_OMNET_UAVS_MODULOCOMUNICACAO_H_

#include "../UAVMessage_m.h"
#include "../UAVMobility.h"

using namespace omnetpp;
using namespace std;


namespace inet {

class ModuloComunicacao : public cSimpleModule {
  protected:
    virtual UAVMessage *generateMessage();
    virtual void forwardMessage(UAVMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Auxiliary functions
    void enviarMensagem(double tempo, int origem, int destino, char const *name, int kind);
    void rememberCheckMessage(double seconds);

    int selfID = -2;
    UAVMessage* sendMSGEvt;
    UAVMobility* getMobility();

  private:
    UAVMobility *mobility;
};
}

#endif
