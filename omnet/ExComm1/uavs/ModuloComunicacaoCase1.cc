#include "ModuloComunicacaoCase1.h"

#include <mysterio/src/mysterio/Mysterio.h>
#include <iostream>
#include <queue>

#include "../../common/msg/MinhaMensagem_m.h"
#include "../../common/msg/StatusModule.h"
#include "../../ExComm1/communication/Loko.h"
#include "../../ExComm1/mysterio/Example1Communication.h"
#include "../../ExComm1/uavs/UAVMobility.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

Define_Module(ModuloComunicacaoCase1);

ModuloComunicacaoCase1::ModuloComunicacaoCase1() { }

ModuloComunicacaoCase1::~ModuloComunicacaoCase1() { }

extern Coord position1[10];
extern double velocidade1[10];

extern int UAVDestino1;
extern int UAVLeader1;

using namespace mysterio;
//extern Mysterio mysterios;
extern Example1Communication mysterios1;
queue<int> filaDeMensagens;

//Aqui usa só getIndex();
//Decidir se usa selfID ou a variável indice
void ModuloComunicacaoCase1::initialize(){
    Loko l;
    mysterios1.conn.sendMessage(&l, &l, 0);

    selfID = getIndex();

    if (selfID == 0) {
        rememberCheckMessage(2);
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
    mysterios1.conn.connectANewUAV(selfID, &mysterios1.status);

    if (selfID == UAVLeader1) {
        cout << "UAV selecionado [" << selfID << "]" << endl;
        int continuar = 0;
        do{
            cout << "\nEscolha o STATUS deseja solicitar:" << std::endl;
            cout << "1. Localizacao" << std::endl;
            cout << "2. Velocidade" << std::endl;

            if (scanf("%d", &continuar) == 1) {
                switch(continuar){
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
        sendMSGEvt->setOrigem(UAVLeader1);
        //cout << "Tipo: " << sendMSGEvt->getKind() << " | " << sendMSGEvt->getName() << endl;

        //Colocar um tempo aleatório em vez de 3
        //Um tempo em que conte um numero random entre 1 e 1+(k/6), onde K = hora
        scheduleAt(simTime()+3.0, sendMSGEvt);
    }
}

void ModuloComunicacaoCase1::rememberCheckMessage(double seconds){
    MinhaMensagem *m = new MinhaMensagem("mandar msg", LEMBRAR);
    scheduleAt(simTime()+seconds, m);
}

void ModuloComunicacaoCase1::handleMessage(cMessage *msg){

    if(selfID == UAVLeader1 && msg->getKind() == LEMBRAR){
        //cout << "!Tamanho da fila antes do front: " << filaDeMensagens.size() << endl;
        if(filaDeMensagens.size() > 0){
            cout << "[U" << selfID << "] Há ";
            cout << filaDeMensagens.front();
            cout << ((filaDeMensagens.front() > 1) ? " mensagens." : " mensagem.") << endl;

            int msgUAV = filaDeMensagens.front();
            filaDeMensagens.pop();

            if(mysterios1.conn.hasMessageToDrone(msgUAV)){
                MinhaMensagem m = mysterios1.conn.receiveMessage(msgUAV);
                cout << "[C2U] Mensagem recebida de " << m.getOrigem();
                cout << " para " << m.getDestino() << endl;
                cout << "[C2U] Kind: " << m.getKind();
                cout << " | titulo: " << m.getTitulo() << endl;
                //mysterios.communication.markAsReceived(msgUAV);
            }else{
                cout << "[C2U] Não há mensagens pro UAV[" << msgUAV << "]" << endl;
            }
        }else{
            cout << "[U" << selfID << "] Não há mensagens." << endl;
        }
        rememberCheckMessage(2);
    }else{
        MinhaMensagem *mMSG = check_and_cast<MinhaMensagem*>(msg);

        cout << "[U2U] Executando ação: " << msg->getFullName() << endl;

        if(selfID == mMSG->getDestino()){
            cout << "[U2U] Chegou no destino" << endl;
            if(msg->getKind() == SOLICITAR_LOCALIZACAO){
                cout << "[U2U] Enviando status(localizacao) de " << selfID << " para " << mMSG->getOrigem() << endl;
                enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando localizacao", RESPONDER_LOCALIZACAO);
                //sendMSGEvt = new MinhaMensagem("Enviando localizacao", RESPONDER_LOCALIZACAO);
                //sendMSGEvt->setDestino(mMSG->getOrigem());
                //sendMSGEvt->setOrigem(selfID);
                //scheduleAt(simTime().dbl()+1.0, sendMSGEvt);
            }else if(msg->getKind() == RESPONDER_LOCALIZACAO){ //Chegou no leader
                StatusModule s = mMSG->getStatus();
                cout << "[U2C] Passando dados para o communication" << endl;
                mysterios1.conn.saveUAVCurrentPosition(mMSG->getOrigem(), s.getLocationX(), s.getLocationY(), s.getLocationZ(), &mysterios1.status);
                cout << "[C2U] Recebendo dados do communication" << endl;
                Coordinate cc = mysterios1.conn.requestUAVCurrentPosition(mMSG->getOrigem(), &mysterios1.status);
                //cout << "COORDENADAS OBTIDAS [" << mMSG->getOrigem() << "]:\nX: " << cc.x << " Y: " << cc.y << " Z: " << cc.z << endl;
                cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << cc.x << " Y: " << cc.y << " Z: " << cc.z << endl;
                //cout << "Localização de [" << mMSG->getOrigem() << "]:\nX: " << s.getLocationX() << " Y: " << s.getLocationY() << " Z: " << s.getLocationZ() << endl;
            }else if(msg->getKind() == SOLICITAR_VELOCIDADE){
                cout << "[U2U] Enviando status(velocidade) de " << selfID << " para " << mMSG->getOrigem() << endl;
                //sendMSGEvt = new MinhaMensagem("Enviando velocidade", RESPONDER_VELOCIDADE);
                //sendMSGEvt->setDestino(mMSG->getOrigem());
                //sendMSGEvt->setOrigem(selfID);
                //scheduleAt(simTime().dbl()+1.0, sendMSGEvt);
                enviarMensagem(1.0, selfID, mMSG->getOrigem(), "Enviando velocidade", RESPONDER_VELOCIDADE);
            }else if(mMSG->getKind() == RESPONDER_VELOCIDADE){
                StatusModule s = mMSG->getStatus();
                cout << "[U2C] Passando dados para o communication" << endl;
                mysterios1.conn.saveUAVCurrentVelocity(mMSG->getOrigem(), s.getVelocity(), &mysterios1.status);
                cout << "[C2U] Recebendo dados do communication" << endl;
                double velocidade = mysterios1.conn.requestUAVCurrentVelocity(mMSG->getOrigem(), &mysterios1.status);
                cout << "[U" << selfID << "] Velocidade de ["<< mMSG->getOrigem() << "]: " << velocidade << " m/s" << endl;
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
                cout << "[U2C] Enviou para o communication: " << UAVDestino1 << " | " << mMSG->getFullName() << endl;
                mMSG->setTitulo(mMSG->getFullName());
                mysterios1.conn.sendMessageDroneToDrone(UAVLeader1, UAVDestino1, mMSG);
                cout << "[C2U] Recebendo dados do communication" << endl;
                filaDeMensagens = mysterios1.conn.messagesToSend();
            }

            StatusModule s;
            switch (msg->getKind()){
                case RESPONDER_LOCALIZACAO:
                    s.setLocation(position1[selfID].x, position1[selfID].y, position1[selfID].z);
                    break;
                case RESPONDER_VELOCIDADE:
                    s.setVelocity(velocidade1[selfID]);
                    break;
                default: /*if(msg) cout << "Nao identificou o tipo da mensagem" << msg->getKind() << endl;*/
                    break;
            }
            mMSG->setStatus(s);

            forwardMessage(mMSG);
        }

    }

}

//ATENÇÃO!!
//Ver
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacaoCase1::forwardMessage(MinhaMensagem *msg){
    //OBS: Aqui o selfID Não é acessado, por isso que é importante usar o getIndex

    //Depois enviar mensagens para todos os vizinhos
    //cout << "O DESTINO É: " << msg->getDestino() << endl;
    //cout << "Passando por " << selfID << endl;
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

void ModuloComunicacaoCase1::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    //MinhaMensagem msg;
    sendMSGEvt = new MinhaMensagem(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}

MinhaMensagem *ModuloComunicacaoCase1::generateMessage(){
    // Produce source and destination addresses.
    int src = getIndex();  // our module index
    int n = getVectorSize();  // module vector size
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
