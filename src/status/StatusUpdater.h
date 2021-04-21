#ifndef MYSTERIO_SRC_STATUS_STATUSUPDATER_H_
#define MYSTERIO_SRC_STATUS_STATUSUPDATER_H_
#include "../utils/UAV.h"

class StatusUpdater {
public:
    StatusUpdater(){ }
    virtual ~StatusUpdater(){ }
    virtual void subscribe(UAV *uav) = 0;
    virtual void requestStatusOnSubscribers() = 0;
    virtual void stopSubscriber(UAV *uav) = 0;
};

#endif
