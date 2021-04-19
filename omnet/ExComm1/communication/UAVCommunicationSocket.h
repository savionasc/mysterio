#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_

#include "../../../src/communication/UAVCommunication.h"

namespace mysterio {

class UAVCommunicationSocket : public UAVCommunication {
public:
    UAVCommunicationSocket(); //Aqqui ele cria a conexão com o Communication
    virtual ~UAVCommunicationSocket();
    void dispatch(int msg);   //Aqui ele envia mensagem pro Communication
};

}

#endif
