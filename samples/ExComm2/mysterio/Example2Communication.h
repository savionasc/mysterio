#ifndef MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE2COMMUNICATION_H_
#define MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE2COMMUNICATION_H_
#include <mysterio/src/mysterio/Mysterio.h>
#include "../communication/Comm2.h"

namespace mysterio {

class Example2Communication {// : public Mysterio {
public:
    Example2Communication();
    virtual ~Example2Communication();
    Comm2 communication;
    Status status;
};

} /* namespace inet */

#endif /* MYSTERIO_SAMPLES_EXCOMM1_MYSTERIO_EXAMPLE1COMMUNICATION_H_ */
