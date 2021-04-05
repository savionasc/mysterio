#ifndef MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_
#define MYSTERIO_SRC_COMMUNICATION_UAVCOMMUNICATION_H_

//Pensei nessa classe como intermediária da Communication e dos UAVs
//Provavelmente ela usará Socket
class UAVCommunication {
public:
    UAVCommunication();
    virtual ~UAVCommunication();
};

#endif
