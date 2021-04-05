#ifndef MYSTERIO_SRC_MYSTERIO_MYSTERIO_H_
#define MYSTERIO_SRC_MYSTERIO_MYSTERIO_H_

#include <mysterio/src/aggregator/Aggregator.h>
#include <mysterio/src/communication/Communication.h>

class Mysterio {
protected:

public:
    Mysterio() {
        // TODO Auto-generated constructor stub
    }

    virtual ~Mysterio() {
        // TODO Auto-generated destructor stub
    }

    Communication communication;
//private: //Eu uso ele para guardar as instâncias da váriavel
    Aggregator informationAggregator; //MUDAR PARA STATUS
};
#endif
