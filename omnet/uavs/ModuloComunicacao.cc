#include "ModuloComunicacao.h"

#include "../communication/UAVCommunicationSocket.h"
#include "../mission/GoToTask.h"
#include "../uavs/UAVMobility.h"
#include <iostream>
#include "../common/DroneStatus.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"

using namespace omnetpp;
using namespace inet;

Define_Module(ModuloComunicacao);

extern Coord position[10];
extern double velocidade[10];
float bateria[10];

extern int UAVDestino;
extern int UAVLeader;

using namespace mysterio;
UAVCommunicationSocket uavs[20];

static GoToTask irNaEsquina;
void ModuloComunicacao::initialize(){
    selfID = getIndex();
    cout << "Bateriaaaa: " << bateria[selfID] << "|" << std::stof(pegarBateria(selfID).str());

    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase(); //Implementar o listen... para ficar ouvindo o canal
    }

    solicitarStatusDoUAVVizinho();
}

void ModuloComunicacao::handleMessage(cMessage *msg){
    DroneMessage *mMSG = check_and_cast<DroneMessage*>(msg);
    cout << "[U2U] Executando ação: " << msg->getFullName() << endl;
    this->atualizarDados();

    if(selfID == mMSG->getDestino()){
        cout << "[U2U] Chegou no destino" << endl;
        if(msg->getKind() == SOLICITAR_LOCALIZACAO){
            cout << "[U2U] Enviando status(localizacao) de " << selfID << " para " << mMSG->getOrigem() << endl;
            enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando localizacao", RESPONDER_LOCALIZACAO);
        }else if(msg->getKind() == RESPONDER_LOCALIZACAO){ //Chegou no leader
            DroneStatus s = mMSG->getStatus();
            cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << s.getLocationX() << " Y: " << s.getLocationX() << " Z: " << s.getLocationX() << endl;
            cout << "[U" << selfID << "] Tempo de Voo de [" << mMSG->getOrigem() << "]:" << s.getFlightTime() << endl;
        }else if(msg->getKind() == SOLICITAR_VELOCIDADE){
            cout << "[U2U] Enviando status(velocidade) de " << selfID << " para " << mMSG->getOrigem() << endl;
            enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando velocidade", RESPONDER_VELOCIDADE);
        }else if(mMSG->getKind() == RESPONDER_VELOCIDADE){
            DroneStatus s = mMSG->getStatus();
            cout << "[U" << selfID << "] Velocidade de ["<< mMSG->getOrigem() << "]: " << s.getVelocity() << " m/s" << endl;
        }else if(msg->getKind() == SOLICITAR_BATERIA){
            this->atualizarDados();
            cout << "[U2U] Enviando status(bateria) de " << selfID << " para " << mMSG->getOrigem() << endl;
            enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando bateria", RESPONDER_BATERIA);
            bateria[selfID] = std::stof(pegarBateria(selfID).str());
        }else if(mMSG->getKind() == RESPONDER_BATERIA){
            DroneStatus s = mMSG->getStatus();
            cout << "[U" << selfID << "] Bateria de ["<< mMSG->getOrigem() << "]: " << s.getBattery() << " J" << endl;
            bateria[selfID] = std::stof(pegarBateria(selfID).str());
        }
    } else {
        if(UAVDestino == -1 && selfID == UAVLeader){
            do{
                cout << "Selecione o UAV de destino." << endl;
                if(scanf("%d", &UAVDestino) != 1){
                    cout << "Execute o programa novamente e digite uma opção valida!" << endl;
                    exit(0);
                }
            } while(UAVDestino <= -1);
            mMSG->setDestino(UAVDestino);
            mMSG->setTitulo(mMSG->getFullName());
            Coordinate currentPosition(100.0, 100.0, 100.0);
            cout << "Resultado da tarefa pro drone [" << selfID << "]: " << irNaEsquina.isComplete(currentPosition) << endl;

        }

        DroneStatus s;
        switch (msg->getKind()){
            case RESPONDER_LOCALIZACAO:
                s.setLocation(position[selfID].x, position[selfID].y, position[selfID].z);
                s.setFlightTime(1500);
                break;
            case RESPONDER_VELOCIDADE:
                s.setVelocity(velocidade[selfID]);
                break;
            case RESPONDER_BATERIA:
                s.setBattery(std::stof(pegarBateria(selfID).str()));
                break;
            default: /*Nao identificou o tipo da mensagem*/
                break;
        }
        mMSG->setStatus(s);
        forwardMessage(mMSG);
    }

}

//ATENÇÃO!!
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacao::forwardMessage(DroneMessage *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

DroneMessage *ModuloComunicacao::generateMessage(){
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "msg-%d-para-%d", src, dest);

    DroneMessage *msg = new DroneMessage(msgname);
    msg->setOrigem(src);
    msg->setDestino(dest);

    return msg;
}

J ModuloComunicacao::pegarBateria(int idUAV){
    cModule *a = getParentModule()->getSubmodule("host", idUAV)->getSubmodule("energyStorage", 0);
    SimpleEpEnergyStorage *energySto = check_and_cast<SimpleEpEnergyStorage*>(a);
    return energySto->getNominalEnergyCapacity();
}

//Para usar com a mensagem //sprintf(msgname, "msg-%d-para-%d", src, dest);

//Auxiliary functions
void ModuloComunicacao::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new DroneMessage(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}

void ModuloComunicacao::solicitarStatusDoUAVVizinho(){

    //Call in the first moments of the application to select a UAV
    if (selfID == 0) {
        cout << "Atribuindo tarefa ao drone [" << selfID << "]" << endl;
        Coordinate c(100.0, 100.0, 100.0);
        irNaEsquina.setTask(c);
        do{
            cout << "Communication Module - Case 1." << endl;
            cout << "Por favor, selecione o UAV leader." << endl;
            cout << "O primeiro UAV é o 0 de tantos" << endl;
            cout << "Diga qual UAV irá fazer algo...:" << endl;
            if (scanf("%d", &UAVLeader) != 1) {
                printf("Execute o programa novamente e digite uma opção valida!\n");
                exit(0);
            }
        } while(UAVLeader <= -1);
    }

    //For the selected UAV
    if (selfID == UAVLeader) {
        cout << "UAV selecionado [" << selfID << "]" << endl;
        int continuar = 0;
        do{
            cout << "\nEscolha o STATUS deseja solicitar:" << std::endl;
            cout << "1. Localizacao" << std::endl;
            cout << "2. Velocidade" << std::endl;
            cout << "3. Bateria" << std::endl;

            if (scanf("%d", &continuar) == 1) {
                switch(continuar){ //Creating Message to communication between UAVs without our framework
                    case 1:
                        sendMSGEvt = new DroneMessage("Solicitando localizacao", SOLICITAR_LOCALIZACAO);
                        break;
                    case 2:
                        sendMSGEvt = new DroneMessage("Solicitando velocidade", SOLICITAR_VELOCIDADE);
                        break;
                    case 3:
                        sendMSGEvt = new DroneMessage("Solicitando bateria", SOLICITAR_BATERIA);
                        break;
                    default:
                        printf("Digite uma opção valida!\n");
                }
            } else {
                printf("Digite uma opção valida!\n");
            }

        } while(continuar <= 0);

        //This message is used only for communication between uavs using just Omnet++
        sendMSGEvt->setOrigem(UAVLeader);
        //Dica para deixar a aplicação aleatória
        //Colocar um tempo aleatório em vez de 3
        //Um tempo em que conte um numero random entre 1 e 1+(k/6), onde K = hora
        scheduleAt(simTime()+3.0, sendMSGEvt);
        bateria[selfID] = std::stof(pegarBateria(selfID).str());
    }
}
void ModuloComunicacao::atualizarDados(){
    for(int i = 0; i < 10; i++)
        bateria[i] = std::stof(pegarBateria(i).str());
}
