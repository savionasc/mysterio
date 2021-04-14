#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_LOKO_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_LOKO_H_
#include <mysterio/src/communication/Communicable.h>
using namespace std;
class Loko : public virtual Communicable {
public:
    Loko();
    virtual ~Loko();
    virtual void onMessageReceive(int a);
};

#endif /* MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_LOKO_H_ */
