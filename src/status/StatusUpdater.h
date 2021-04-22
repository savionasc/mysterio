#ifndef MYSTERIO_SRC_STATUS_STATUSUPDATER_H_
#define MYSTERIO_SRC_STATUS_STATUSUPDATER_H_
#include "../utils/UAV.h"
#include <iostream>
#include <list>

class StatusUpdater {
public:
    //StatusUpdater(){ }
    //virtual ~StatusUpdater(){ }
    virtual void subscribe(UAV *uav) = 0;
    virtual void requestStatusOnSubscribers() = 0;
    virtual std::list<UAV*> getListSubscribers() = 0;
    virtual void stopSubscriber(UAV *uav) = 0;
};

#endif
