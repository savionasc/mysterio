#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLEIMPL_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLEIMPL_H_

#include "Communicable.h"

class CommunicableImpl : public Communicable {
public:
    void onMessageReceive();
};

#endif /* MYSTERIO_SRC_COMMUNICATION_COMMUNICABLEIMPL_H_ */
