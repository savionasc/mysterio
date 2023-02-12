#include "ModuloComunicacao.h"

#include "../../mission/GoTo.h"
#include "../UAVMobility.h"
#include <iostream>

#include "../../../src/status/UAVStatus.h"
#include "../../communication/uav/UAVDispatcher.h"

using namespace omnetpp;
using namespace inet;
using namespace mysterio;

Define_Module(ModuloComunicacao);

//shared variables
extern Coord position[NUMUAVS];
extern double velocidade[NUMUAVS];
extern float bateria[NUMUAVS];
extern double tempoVoo[NUMUAVS];
//extern UAVDispatcher uavs[NUMUAVS];
extern int UAVDestino;
extern int UAVLeader;

void ModuloComunicacao::initialize(){
    selfID = getIndex();
    mobility = getMobility();
    mulambo = 67+selfID;

    /*mobility->connUAV.setSelfID(selfID);*/

    //uavs[selfID].setSelfID(selfID);

//    if(!uavs[selfID].isConnected()){
//        uavs[selfID].connectBase();
//    }

    /*if(!mobility->connUAV.isConnected()){
        mobility->connUAV.connectBase();
    }*/
    bubble("INICIANDO, Iniciando!");
    sendMSGEvt = new UAVMessage("Opa", 0);
    sendMSGEvt->setDestino(selfID);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+1, sendMSGEvt);
}

void ModuloComunicacao::handleMessage(cMessage *msg){
    /*if(selfID == 1){
        cSimulation *currentSimulation = getSimulation();
        inet::UAVMobility *mob = nullptr;
        //inet::UAVMobility*
        mob = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath("MysterioCommunication1.UAV[0].mobility"));
        cout << mob->getModuleType() << endl;
        //(omnetpp::cDynamicModuleType)mysterio.connors.uavs.UAVMobility
        cout << "[comunicacao-uav-"<< selfID <<"] Pegou cabron de: " << mob->uav.getID() << " Cabron: " << mob->cabron << endl;

        ModuloComunicacao2 *com = check_and_cast<ModuloComunicacao2 *>(currentSimulation->getModuleByPath("MysterioCommunication1.UAV[0]"));
        cout << com->getModuleType() << endl;
        //(omnetpp::cDynamicModuleType)mysterio.connors.uavs.UAVMobility
        cout << "[comunicacao-uav-"<< selfID <<"] Pegou cabron de: " << com->selfID << " mulambo: " << com->mulambo << endl;
    }*/


        ///AQUIIIIII
        /*int x = par("savio");
       int random = 1+(rand() % 100);
       cout << "Savio: " << random << endl;
       par("savio").setIntValue(random);
       sendMSGEvt = new UAVMessage("Opa", 0);
       sendMSGEvt->setDestino(selfID);
       sendMSGEvt->setOrigem(selfID);
       scheduleAt(simTime()+3, sendMSGEvt);*/






    // UAVMessage *mMSG = check_and    _cast<UAVMessage*>(msg);
    // cout << "[U2U] Executando ação: " << msg->getFullName() << endl;

    // if(selfID == mMSG->getDestino()){
    //     cout << "[U2U] Chegou no destino" << endl;
    //     if(msg->getKind() == SOLICITAR_LOCALIZACAO){
    //         cout << "[U2U] Enviando status(localizacao) de " << selfID << " para " << mMSG->getOrigem() << endl;
    //         enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando localizacao", RESPONDER_LOCALIZACAO);
    //     }else if(msg->getKind() == RESPONDER_LOCALIZACAO){ //Chegou no leader
    //         DroneStatus s = mMSG->getStatus();
    //         cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << s.getLocationX() << " Y: " << s.getLocationX() << " Z: " << s.getLocationX() << endl;
    //         this->solicitarStatusDoUAVVizinho();
    //     }else if(msg->getKind() == SOLICITAR_VELOCIDADE){
    //         cout << "[U2U] Enviando status(velocidade) de " << selfID << " para " << mMSG->getOrigem() << endl;
    //         enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando velocidade", RESPONDER_VELOCIDADE);
    //     }else if(mMSG->getKind() == RESPONDER_VELOCIDADE){
    //         DroneStatus s = mMSG->getStatus();
    //         cout << "[U" << selfID << "] Velocidade de ["<< mMSG->getOrigem() << "]: " << s.getVelocity() << " m/s" << endl << endl;
    //         this->solicitarStatusDoUAVVizinho();
    //     }else if(msg->getKind() == SOLICITAR_BATERIA){
    //         cout << "[U2U] Enviando status(bateria) de " << selfID << " para " << mMSG->getOrigem() << endl;
    //         enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando bateria", RESPONDER_BATERIA);
    //     }else if(mMSG->getKind() == RESPONDER_BATERIA){
    //         DroneStatus s = mMSG->getStatus();
    //         cout << "[U" << selfID << "] Bateria de ["<< mMSG->getOrigem() << "]: " << s.getBattery() << " J" << endl << endl;
    //         this->solicitarStatusDoUAVVizinho();
    //     }else if(msg->getKind() == SOLICITAR_TEMPOVOO){
    //         cout << "[U2U] Enviando status(tempo de voo) de " << selfID << " para " << mMSG->getOrigem() << endl;
    //         enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando tempo de voo", RESPONDER_TEMPOVOO);
    //     }else if(mMSG->getKind() == RESPONDER_TEMPOVOO){
    //         DroneStatus s = mMSG->getStatus();
    //         cout << "[U" << selfID << "] Tempo de voo de ["<< mMSG->getOrigem() << "]: " << s.getFlightTime() << " s" << endl << endl;
    //         this->solicitarStatusDoUAVVizinho();
    //     }else if(mMSG->getKind() == GOTOTASK){
    //         cout << "Atribuindo tarefa ao drone [" << selfID << "]" << endl;
    //         Coordinate c(100.0, 100.0, 100.0);
    //         minhasTarefas[selfID].started = true;
    //         minhasTarefas[selfID].setTask(c);
    //     }else if(mMSG->getKind() == TASKCOMPLETED){
    //         Coordinate currentPosition(100.0, 100.0, 100.0);
    //         cout << "Resultado da tarefa pro drone [" << selfID << "]: " << minhasTarefas[selfID].isComplete(currentPosition) << endl;
    //     }
    // } else {
    //     if(UAVDestino == -1 && selfID == UAVLeader){
    //         do{
    //             cout << "Selecione o UAV de destino." << endl;
    //             if(scanf("%d", &UAVDestino) != 1){
    //                 cout << "Execute o programa novamente e digite uma opção valida!" << endl;
    //                 exit(0);
    //             }
    //         } while(UAVDestino <= -1);
    //         mMSG->setDestino(UAVDestino);
    //         mMSG->setTitulo(mMSG->getFullName());
    //     }

    //     DroneStatus s;
    //     switch (msg->getKind()){
    //         case RESPONDER_LOCALIZACAO:
    //             s.setLocation(position[selfID].x, position[selfID].y, position[selfID].z);
    //             break;
    //         case RESPONDER_VELOCIDADE:
    //             s.setVelocity(velocidade[selfID]);
    //             break;
    //         case RESPONDER_BATERIA:
    //             s.setBattery(bateria[selfID]);
    //             break;
    //         case RESPONDER_TEMPOVOO:
    //             s.setFlightTime(tempoVoo[selfID]);
    //             break;
    //         default: //Nao identificou o tipo da mensagem
    //             break;
    //     }
    //     mMSG->setStatus(s);
    //     forwardMessage(mMSG);
    // }

}

//ATENÇÃO!!
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacao::forwardMessage(UAVMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

UAVMessage *ModuloComunicacao::generateMessage(){
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "msg-%d-para-%d", src, dest);

    UAVMessage *msg = new UAVMessage(msgname);
    msg->setOrigem(src);
    msg->setDestino(dest);

    return msg;
}

//Para usar com a mensagem //sprintf(msgname, "msg-%d-para-%d", src, dest);

//Auxiliary functions
void ModuloComunicacao::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new UAVMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}

void ModuloComunicacao::solicitarStatusDoUAVVizinho(){
    // UAVDestino = -1;
    // //Call in the first moments of the application to select a UAV
    // if (selfID == 0) {
    //     do{
    //         cout << "Communication Module - Case 1." << endl;
    //         cout << "Por favor, selecione o UAV leader." << endl;
    //         cout << "O primeiro UAV é o 0 de tantos" << endl;
    //         cout << "Diga qual UAV irá fazer algo...:" << endl;
    //         if (scanf("%d", &UAVLeader) != 1) {
    //             printf("Execute o programa novamente e digite uma opção valida!\n");
    //             exit(0);
    //         }
    //     } while(UAVLeader <= -1);
    // }

    // //For the selected UAV
    // if (selfID == UAVLeader) {
    //     cout << "UAV selecionado [" << selfID << "]" << endl;
    //     int continuar = 0;
    //     do{
    //         cout << "\nEscolha o STATUS deseja solicitar:" << std::endl;
    //         cout << "1. Localizacao" << std::endl;
    //         cout << "2. Velocidade" << std::endl;
    //         cout << "3. Bateria" << std::endl;
    //         cout << "4. Tempo de voo" << std::endl;
    //         cout << "5. Go to Task" << std::endl;
    //         cout << "6. Task Completed?" << std::endl;
    //         cout << "9. Para adiar" << std::endl;


    //         if (scanf("%d", &continuar) == 1) {
    //             switch(continuar){ //Creating Message to communication between UAVs without our framework
    //                 case 1:
    //                     sendMSGEvt = new UAVMessage("Solicitando localizacao", SOLICITAR_LOCALIZACAO);
    //                     break;
    //                 case 2:
    //                     sendMSGEvt = new UAVMessage("Solicitando velocidade", SOLICITAR_VELOCIDADE);
    //                     break;
    //                 case 3:
    //                     sendMSGEvt = new UAVMessage("Solicitando bateria", SOLICITAR_BATERIA);
    //                     break;
    //                 case 4:
    //                     sendMSGEvt = new UAVMessage("Solicitando tempo de voo", SOLICITAR_TEMPOVOO);
    //                     break;
    //                 case 5:
    //                     sendMSGEvt = new UAVMessage("Go To - Task", GOTOTASK);
    //                     break;
    //                 case 6:
    //                     sendMSGEvt = new UAVMessage("Task Completed?", TASKCOMPLETED);
    //                     break;
    //                 default:
    //                     printf("Digite uma opção valida!\n");
    //             }
    //         } else {
    //             printf("Digite uma opção valida!\n");
    //         }

    //     } while(continuar <= 0);

    //     if(continuar != 9){
    //         //This message is used only for communication between uavs using just Omnet++
    //         sendMSGEvt->setOrigem(UAVLeader);
    //         //Dica para deixar a aplicação aleatória
    //         //Colocar um tempo aleatório em vez de 3
    //         //Um tempo em que conte um numero random entre 1 e 1+(k/6), onde K = hora
    //         scheduleAt(simTime()+7.0, sendMSGEvt);
    //     }else{
    //         //no else escalonar uma mensagem para lembrar de perguntar alguma coisa...
    //     }
    // }
}

UAVMobility* ModuloComunicacao::getMobility(){
    cSimulation *currentSimulation = getSimulation();
    UAVMobility *mob = nullptr;
    string str = "MysterioCommunication.UAV["+to_string(getIndex())+"].mobility";
    cout << "Opa" << endl;
    cout << str << endl;
    char path[str.length()+1];
    strcpy(path, str.c_str());
    //mob = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath("MysterioCommunication.UAV[0].mobility"));
    mob = check_and_cast<UAVMobility *>(currentSimulation->getModuleByPath(path));

    //cout << mob->getModuleType() << endl;
    //(omnetpp::cDynamicModuleType)mysterio.connors.uavs.UAVMobility
    //cout << "[comunicacao-uav-"<< selfID <<"] Pegou cabron de: " << mob->uav.getID() << " Cabron: " << mob->cabron << endl;

    //ModuloComunicacao2 *com = check_and_cast<ModuloComunicacao2 *>(currentSimulation->getModuleByPath("MysterioCommunication1.UAV[0]"));
    //cout << com->getModuleType() << endl;
    //(omnetpp::cDynamicModuleType)mysterio.connors.uavs.UAVMobility
    //cout << "[comunicacao-uav-"<< selfID <<"] Pegou cabron de: " << com->selfID << " mulambo: " << com->mulambo << endl;
    return mob;
}
