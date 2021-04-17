#ifndef MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#define MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
//#include <mysterio/src/mysterio/Mysterio.h>
#include "../communication/CommunicationSocket.h"
#include "../status/StatusC1.h"
//#include <mysterio/src/communication/Testam.h>

namespace mysterio {

class Example1Communication {
public:
    Example1Communication();
    virtual ~Example1Communication();
    CommunicationSocket conn;
//private: //Eu uso ele para guardar as instâncias da váriavel
    StatusC1 status; //MUDAR PARA STATUS
};

} /* namespace inet */

#endif /* MYSTERIO_OMNET_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_ */
