#include "ModuloComunicacaoCase1.h"
#include "../common/msg/StatusModule.h"
#include "../communication/UAVCommunicationSocket.h"
#include "../mission/GoToTask.h"
#include "../uavs/UAVMobility.h"
#include <iostream>

using namespace omnetpp;
using namespace inet;

Define_Module(ModuloComunicacaoCase1);

extern Coord position1[10];
extern double velocidade1[10];

extern int UAVDestino1;
extern int UAVLeader1;

using namespace mysterio;
UAVCommunicationSocket uavs[20];

static GoToTask irNaEsquina;
//Aqui usa só getIndex();
void ModuloComunicacaoCase1::initialize(){
    selfID = getIndex();
    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase(); //Implementar o listen... para ficar ouvindo o canal
    }

    solicitarStatusDoUAVVizinho();
}

void ModuloComunicacaoCase1::handleMessage(cMessage *msg){
    MinhaMensagem *mMSG = check_and_cast<MinhaMensagem*>(msg);

    cout << "[U2U] Executando ação: " << msg->getFullName() << endl;

    if(selfID == mMSG->getDestino()){
        cout << "[U2U] Chegou no destino" << endl;
        if(msg->getKind() == SOLICITAR_LOCALIZACAO){
            cout << "[U2U] Enviando status(localizacao) de " << selfID << " para " << mMSG->getOrigem() << endl;
            enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando localizacao", RESPONDER_LOCALIZACAO);
        }else if(msg->getKind() == RESPONDER_LOCALIZACAO){ //Chegou no leader
            StatusModule s = mMSG->getStatus();
            cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << s.getLocationX() << " Y: " << s.getLocationX() << " Z: " << s.getLocationX() << endl;
        }else if(msg->getKind() == SOLICITAR_VELOCIDADE){
            cout << "[U2U] Enviando status(velocidade) de " << selfID << " para " << mMSG->getOrigem() << endl;
            enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando velocidade", RESPONDER_VELOCIDADE);
        }else if(mMSG->getKind() == RESPONDER_VELOCIDADE){
            StatusModule s = mMSG->getStatus();
            cout << "[U" << selfID << "] Velocidade de ["<< mMSG->getOrigem() << "]: " << s.getVelocity() << " m/s" << endl;
        }
    } else {
        if(UAVDestino1 == -1 && selfID == UAVLeader1){
            do{
                cout << "Selecione o UAV de destino." << endl;
                if(scanf("%d", &UAVDestino1) != 1){
                    cout << "Execute o programa novamente e digite uma opção valida!" << endl;
                    exit(0);
                }
            } while(UAVDestino1 <= -1);
            mMSG->setDestino(UAVDestino1);
            mMSG->setTitulo(mMSG->getFullName());
            Coordinate currentPosition(100.0, 100.0, 100.0);
            cout << "Resultado da tarefa pro drone [" << selfID << "]: " << irNaEsquina.isComplete(currentPosition) << endl;

        }

        StatusModule s;
        switch (msg->getKind()){
            case RESPONDER_LOCALIZACAO:
                s.setLocation(position1[selfID].x, position1[selfID].y, position1[selfID].z);
                break;
            case RESPONDER_VELOCIDADE:
                s.setVelocity(velocidade1[selfID]);
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

void ModuloComunicacaoCase1::forwardMessage(MinhaMensagem *msg){
    //Depois enviar mensagens para todos os vizinhos
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

MinhaMensagem *ModuloComunicacaoCase1::generateMessage(){
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "msg-%d-para-%d", src, dest);

    MinhaMensagem *msg = new MinhaMensagem(msgname);
    msg->setOrigem(src);
    msg->setDestino(dest);

    return msg;
}


//Para usar com a mensagem //sprintf(msgname, "msg-%d-para-%d", src, dest);

//Auxiliary functions
void ModuloComunicacaoCase1::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    sendMSGEvt = new MinhaMensagem(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}

void ModuloComunicacaoCase1::solicitarStatusDoUAVVizinho(){

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
            if (scanf("%d", &UAVLeader1) != 1) {
                printf("Execute o programa novamente e digite uma opção valida!\n");
                exit(0);
            }
        } while(UAVLeader1 <= -1);
    }

    //For the selected UAV
    if (selfID == UAVLeader1) {
        cout << "UAV selecionado [" << selfID << "]" << endl;
        int continuar = 0;
        do{
            cout << "\nEscolha o STATUS deseja solicitar:" << std::endl;
            cout << "1. Localizacao" << std::endl;
            cout << "2. Velocidade" << std::endl;

            if (scanf("%d", &continuar) == 1) {
                switch(continuar){ //Creating Message to communication between UAVs without our framework
                    case 1:
                        sendMSGEvt = new MinhaMensagem("Solicitando localizacao", SOLICITAR_LOCALIZACAO);
                        break;
                    case 2:
                        sendMSGEvt = new MinhaMensagem("Solicitando velocidade", SOLICITAR_VELOCIDADE);
                        break;
                    default:
                        printf("Digite uma opção valida!\n");
                }
            } else {
                printf("Digite uma opção valida!\n");
            }

        } while(continuar <= 0);

        //This message is used only for communication between uavs using just Omnet++
        sendMSGEvt->setOrigem(UAVLeader1);
        //Dica para deixar a aplicação aleatória
        //Colocar um tempo aleatório em vez de 3
        //Um tempo em que conte um numero random entre 1 e 1+(k/6), onde K = hora
        scheduleAt(simTime()+3.0, sendMSGEvt);
    }
}
