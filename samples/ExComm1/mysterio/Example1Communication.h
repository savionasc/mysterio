#ifndef MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
#define MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_
//#include <mysterio/src/mysterio/Mysterio.h>
#include "../communication/Comm.h"
//#include <mysterio/src/communication/Testam.h>

namespace mysterio {

class Example1Communication {
public:
    Example1Communication();
    virtual ~Example1Communication();
    Comm conn;
//private: //Eu uso ele para guardar as instâncias da váriavel
    Status status; //MUDAR PARA STATUS
};

} /* namespace inet */

#endif /* MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_ */
