#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_COMMUNICATIONSOCKET_H_

#include "../../osborn/status/MysStatusManager.h"
#include "../../src/utils/Coordinate.h"
#include "../../src/communication/Communication.h"
#include "../../src/communication/Communicable.h"
#include "../../src/communication/Message.h"

class MysCommunication : public Communication {
public:

    void ReceiveMessageFromUAV(Communicable *source, Communicable *dest, Message msg);
    int* getActiveConnections(); 
    void sendMessageToUAV(int id, Message msg);

    int configureConnection(int connection){ return connection; }

    int countActiveConnections(){
        MysStatusManager *ms;
        return ms->getSize();
    }

    int configureSocketServer(int port);

    void sendTaskMessageToUAV(int idSocket, TaskMessage tmsg);

    thread listenForNewConnections();

    void reconnect(UAV u){
    }

    int getPortServer() {
        return portServer;
    }

    void setPortServer(int portServer) {
        this->portServer = portServer;
    }

private:
    int portServer;
};

#endif
