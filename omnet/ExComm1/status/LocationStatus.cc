#include "LocationStatus.h"
#include <bits/stdc++.h>

LocationStatus::LocationStatus() { }

LocationStatus::~LocationStatus() { }

void LocationStatus::subscribe(UAV *uav){
    this->subscribers.push_back(uav);
}

void LocationStatus::requestStatusOnSubscribers(){
    CommunicationSocket cs;
    for (UAV *u : this->subscribers) {
        std::cout << u->getID() << std::endl;
        //               this    uav   msg
        //cs.sendMessage(source, dest, msg);
    }
}

void LocationStatus::stopSubscriber(UAV *uav){
    this->subscribers.remove(uav);
}
