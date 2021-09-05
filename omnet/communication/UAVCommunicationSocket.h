#ifndef MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#define MYSTERIO_OMNET_EXCOMM1_COMMUNICATION_UAVCOMMUNICATIONSOCKET_H_
#include <thread>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../src/communication/UAVCommunication.h"
#include "CommunicationSocket.h"
#include "../../src/utils/Message.h"
#include "../../omnet/communication/DroneStatusMessage.h"
#include "../communication/DroneStatusMessage.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/power/base/EpEnergyStorageBase.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"
#include "../mission/GoTo.h"

using namespace inet;
using namespace std;

extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
//extern std::vector<Task*> base[NUMUAVS]; //Task
extern std::vector<Task> base[NUMUAVS]; //Task
extern bool ativo[NUMUAVS];
extern int itera[NUMUAVS];

namespace mysterio {

class UAVCommunicationSocket : public UAVCommunication {
    friend class SocketMessageReceive;
public:
    //UAVCommunication
    void connectBase();
    void dispatchStatusMessage(DroneStatusMessage msg);
    void dispatchMessage(Message msg); //Aqui ele deve enviar mensagem pro Communication
    void disconnectBase();

    int  conexao();
    int  getSocketCode();
    bool isConnected();
    void setConnected(bool connected);
    void setSocketCode(int socketCode);
    int  getSelfID();
    void setSelfID(int selfID);
protected:
    thread receber;
private:
    bool connected = false;
    int socketCode = -1;
    int selfID = -1;
};
}

#endif
