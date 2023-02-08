#include "./CommunicationModule.h"

using namespace omnetpp;
using namespace inet;
//using namespace mysterio;

Define_Module(CommunicationModule);

enum codesUAV{
    MESSAGE_CODE = 123,
    REQUEST_INFORMATION = 234
};

void CommunicationModule::initialize(){
    selfID = getIndex();
    cout << "Simulation started. Message from UAV" << selfID << endl;


    // mysterioConnection[selfID].setSelfID(selfID);

    // if(!mysterioConnection[selfID].isConnected()){
    //    mysterioConnection[selfID].connectBase();
    // }
    cout << "Communication started!" << endl;
}

void CommunicationModule::handleMessage(cMessage *msg){
    //Casting to Mysterio Message Type
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);

    // Make some steps...
    //...

    delete mMSG;
}

void CommunicationModule::forwardMessage(UAVMessage *msg) {
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *CommunicationModule::generateMessage(){
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "msg-%d-para-%d", src, dest);

    UAVMessage *msg = new UAVMessage(msgname);
    msg->setSource(src);
    msg->setDestination(dest);

    return msg;
}
