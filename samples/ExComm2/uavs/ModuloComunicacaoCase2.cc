#include "ModuloComunicacaoCase2.h"
#include <iostream>
#include <queue>

#include "../mysterio/Example2Communication.h"
#include "../../common/msg/MinhaMensagem_m.h"
#include "../../common/msg/Status.h"
#include "UAVMobility2.h"
#include "../mysterio/Banco.h"

using namespace omnetpp;
using namespace std;
using namespace inet;

Define_Module(ModuloComunicacaoCase2);

ModuloComunicacaoCase2::ModuloComunicacaoCase2() {
    // TODO Auto-generated constructor stub
}

ModuloComunicacaoCase2::~ModuloComunicacaoCase2() {
    // TODO Auto-generated destructor stub
}

extern Coord position2[10];
extern double velocidade2[10];

extern int UAVOrigem2;
extern int UAVDestino2;

using namespace mysterio;
extern Example2Communication mysterios2;

void ModuloComunicacaoCase2::initialize(){
    selfID = getIndex();

    rememberCheckMessage(2);
    if (selfID == 0) {
        do{
            cout << "Communication Module - Case 2." << endl;
            cout << "Por favor, selecione o UAV que irá mandar mensagem." << endl;
            cout << "O primeiro UAV é o 0." << endl;
            cout << "Diga qual UAV irá fazer algo...:" << endl;
            if (scanf("%d", &UAVOrigem2) != 1) {
                printf("Execute o programa novamente e digite uma opção valida!\n");
                exit(0);
            }
        } while(UAVOrigem2 <= -1);
    }
    mysterios2.communication.connectANewUAV(selfID, &mysterios2.informationAggregator);

    if (selfID == UAVOrigem2) {
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
        sendMSGEvt->setOrigem(UAVOrigem2);
        if(UAVDestino2 == -1){
            do{
                cout << "Selecione o UAV de destino." << endl;
                if(scanf("%d", &UAVDestino2) != 1){
                    cout << "Execute o programa novamente e digite uma opção valida!" << endl;
                    exit(0);
                }
            } while(UAVDestino2 <= -1);
            sendMSGEvt->setDestino(UAVDestino2);
        }

        cout << "[U2C] Mensagem envianda de " << selfID << " para o communication com destino " << UAVDestino2 << " | " << sendMSGEvt->getFullName() << endl;
        mysterios2.communication.sendMessageDroneToDrone(UAVOrigem2, UAVDestino2, sendMSGEvt);
    }
}

void ModuloComunicacaoCase2::rememberCheckMessage(double seconds){//, MinhaMensagem *m){
    MinhaMensagem *m = new MinhaMensagem("mandar msg", LEMBRAR);
    scheduleAt(simTime()+seconds, m);
}

void ModuloComunicacaoCase2::handleMessage(cMessage *msg){
    MinhaMensagem *mMSG = check_and_cast<MinhaMensagem*>(msg);

    if(mysterios2.communication.hasMessageToDrone(selfID)){ //Verificando as mensagens
        cout << "[U" << selfID << "] Há mensagens pro UAV [" << selfID << "]" << endl;
        *mMSG = mysterios2.communication.receiveMessage(selfID);
        cout << "[C2U] Mensagem recebida de " << mMSG->getOrigem();
        cout << " para " << mMSG->getDestino() << endl;
        cout << "[C2U] Kind: " << mMSG->getKind();
        cout << " | titulo: " << mMSG->getTitulo() << endl;
        mysterios2.communication.markAsReceived(selfID);
    }else{
        rememberCheckMessage(2);
        //cout << "Não há mensagens para " << selfID << endl;
    }

    if(mMSG->getKind() == SOLICITAR_LOCALIZACAO){
        cout << "[U2C] Enviando status(localizacao) de " << selfID << " para o communication com destino " << UAVOrigem2 << " | " << mMSG->getFullName() << endl;
        mMSG->setTitulo("Respondendo localização");
        mMSG->setKind(RESPONDER_LOCALIZACAO);
        mMSG->setDestino(mMSG->getOrigem());
        mMSG->setOrigem(selfID);
        Status s;
        s.setLocation(position2[selfID].x, position2[selfID].y, position2[selfID].z);
        mMSG->setStatus(s);
        cout << "[U2C] Passando dados para o communication" << endl;
        mysterios2.communication.sendMessageDroneToDrone(UAVDestino2, UAVOrigem2, mMSG);
        //cout << "[C2U] Recebendo dados do communication" << endl;


        //Status s;
        //s = mMSG->getStatus();
        //cout << "[U2C] Passando dados para o communication" << endl;
        //mysterios2.communication.saveUAVCurrentPosition(mMSG->getOrigem(), s.getLocationX(), s.getLocationY(), s.getLocationZ(), &mysterios2.informationAggregator);
        //cout << "[C2U] Recebendo dados do communication" << endl;
        //Coordinate cc = mysterios2.communication.requestUAVCurrentPosition(mMSG->getOrigem(), &mysterios2.informationAggregator);
        //cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << cc.x << " Y: " << cc.y << " Z: " << cc.z << endl;

    }else if(mMSG->getKind() == RESPONDER_LOCALIZACAO){
        Status s = mMSG->getStatus();
        cout << "[U" << selfID << "] Localização de [" << mMSG->getOrigem() << "]:\n   X: " << s.getLocationX() << " Y: " << s.getLocationY() << " Z: " << s.getLocationZ() << endl;
        Banco b;
        //Coordinate c(s.getLocationX(), s.getLocationY(), s.getLocationZ());
        Coordinate c;
        c.x = s.getLocationX();
        c.y = s.getLocationY();
        c.z = s.getLocationZ();
        b.testeConexao();
        b.insertLocation(c, mMSG->getOrigem());
        cout << "AAAAAAAA" << endl;
    }else if(mMSG->getKind() == SOLICITAR_VELOCIDADE){
        cout << "[U2C] Enviando status(velocidade) de " << selfID << " para o communication com destino " << UAVOrigem2 << " | " << mMSG->getFullName() << endl;
        mMSG->setTitulo("Enviando velocidade");
        mMSG->setKind(RESPONDER_VELOCIDADE);
        mMSG->setDestino(mMSG->getOrigem());
        mMSG->setOrigem(selfID);
        Status s;
        s.setVelocity(velocidade2[selfID]);
        mMSG->setStatus(s);
        cout << "[U2C] Passando dados para o communication" << endl;
        mysterios2.communication.sendMessageDroneToDrone(UAVDestino2, UAVOrigem2, mMSG);
    }else if(mMSG->getKind() == RESPONDER_VELOCIDADE){
        Status s = mMSG->getStatus();
        cout << "Velocidade de ["<< mMSG->getOrigem() << "]: " << s.getVelocity() << " m/s" << endl;
        //Status s = mMSG->getStatus();
        //cout << "[U2C] Passando dados para o communication" << endl;
        //mysterios2.communication.saveUAVCurrentVelocity(mMSG->getOrigem(), s.getVelocity(), &mysterios2.informationAggregator);
        //cout << "[C2U] Recebendo dados do communication" << endl;
        //double velocidade = mysterios2.communication.requestUAVCurrentVelocity(mMSG->getOrigem(), &mysterios2.informationAggregator);
        //cout << "[U" << selfID << "] Velocidade de ["<< mMSG->getOrigem() << "]: " << velocidade << " m/s" << endl;
    }

    Status s;
    switch (mMSG->getKind()){
        case SOLICITAR_LOCALIZACAO:

            break;
        case SOLICITAR_VELOCIDADE:
            s.setVelocity(velocidade2[selfID]);
            break;
        default: /*if(msg) cout << "Nao identificou o tipo da mensagem" << msg->getKind() << endl;*/
            break;
    }


    //forwardMessage(mMSG);
}

//ATENÇÃO!!
//Ver
//Acho que sprintf(msgname, "msg-%d-para-%d", src, dest); mostra na tela um texto na mensagem
//Depois usar bubble("CHEGOU, gostei do recebido!"); que ele mostra na interface gráfica que chegou a mensagem!

void ModuloComunicacaoCase2::forwardMessage(MinhaMensagem *msg){
    //OBS: Aqui o selfID Não é acessado, por isso que é importante usar o getIndex
    // In this example, we just pick a random gate to send it on.
    // We draw a random number between 0 and the size of gate `out[]'.

    //Depois enviar mensagens para todos os vizinhos
    cout << "O DESTINO É: " << msg->getDestino() << endl;
    cout << "Passando por " << selfID << endl;
    int n = gateSize("out");
    int k = intuniform(0, (n-1));

    EV << "[meu]Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

void ModuloComunicacaoCase2::enviarMensagem(double tempo, int origem, int destino, char const *name, int kind){
    //MinhaMensagem msg;
    sendMSGEvt = new MinhaMensagem(name, kind);
    sendMSGEvt->setDestino(destino);
    sendMSGEvt->setOrigem(origem);
    scheduleAt(simTime()+tempo, sendMSGEvt);
}


MinhaMensagem *ModuloComunicacaoCase2::generateMessage(){
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

