#include "../../osborn/uavs/ModuloComunicacao.h"


using namespace omnetpp;
using namespace inet;
using namespace mysterio;

Define_Module(ModuloComunicacao);

enum codesUAV{
    CHECKING_MESSAGE = 123,
    REQUEST_POSITION_UAV = 234,
    RESPONSE_POSITION_UAV,
    REQUEST_CONSENSUS = 244,
    RESPONSE_CONSENSUS,
    TASK_WAITTING = 345,
    TASK_COMPLETED
};

void ModuloComunicacao::initialize(){
    selfID = getIndex();

    uavs[selfID].setSelfID(selfID);

    if(!uavs[selfID].isConnected()){
        uavs[selfID].connectBase();
    }
    cout << "Iniciou comunicação UAV!" << endl;

    //Iniciando procura por novas mensagens
    UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
    sendMSGEvt->setOrigem(selfID);
    scheduleAt(simTime()+2, sendMSGEvt);
}

void ModuloComunicacao::handleMessage(cMessage *msg){
    UAVMessage *mMSG = check_and_cast<UAVMessage*>(msg);

    handleNessagesBetweenUAVs(mMSG);

    handleNessagesBetweenModules(mMSG);

    delete mMSG;
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

void ModuloComunicacao::enviarMensagemParaTodosOsUAVsAtivos(UAVMessage *msg){
    cout << "ParaTodosOsUAVsAtivos de: " << selfID << endl;
    //Depois enviar mensagens para todos os vizinhos
    //int n = gateSize("out");
    int n = getUAVsAtivos();
    int id = 0;
    for (int i = 0; i < n; i++) {

        if(selfID == id && n-1 > i){
            id += 1;
        }

        msg->setDestino(id);
        send(msg->dup(), "out", i);
        id++;
    }

}

void ModuloComunicacao::enviarMensagemParaTodosOsUAVs(UAVMessage *msg, int size = 0){
    cout << "ParaTodosOsUAVs de: " << selfID << endl;
    //Depois enviar mensagens para todos os vizinhos
    int n;
    if(size == 0){
        n = gateSize("out");
    }else{
        n = size;
    }

    int id = 0;
    for (int i = 0; i < n; i++) {

        if(selfID == id && n-1 > i){
            id += 1;
        }

        msg->setDestino(id);
        send(msg->dup(), "out", i);
        id++;
    }

}


void ModuloComunicacao::handleNessagesBetweenUAVs(UAVMessage *mMSG){
    //SE MENSAGEM RECEBIDA POR OUTRO UAV
        if(mMSG->getKind() == REQUEST_POSITION_UAV && strcmp(mMSG->getName(), "location") == 0){
            //cout << "Mensagem recebida em: " << selfID << " de: " << mMSG->getOrigem();
            //cout << " Tipo: " << mMSG->getKind() << endl;

            //RESPONDENDO
            UAVMessage *uavMSG = new UAVMessage("location", RESPONSE_POSITION_UAV);
            uavMSG->setDestino(mMSG->getOrigem());
            uavMSG->setOrigem(selfID);
            uavMSG->setStatus(UAVStatus(castCoordToCoordinate(position[selfID])));
            if(uavMSG->getDestino() > selfID){
                send(uavMSG, "out", uavMSG->getDestino()-1);
            }else{
                send(uavMSG, "out", uavMSG->getDestino());
            }
            //UAVStatus us = uavMSG->getStatus();
            //cout << "[REQUEST][UAV"<< mMSG->getOrigem() <<"-STATUS-TO-U"<<selfID<<"] Status recebido: x:" << us.getLocationX();
            //cout << " y: " << us.getLocationY();
            //cout << " z: " << us.getLocationZ() << endl;
        }else if(mMSG->getKind() == RESPONSE_POSITION_UAV){
            //UAVStatus us = mMSG->getStatus();
            //cout << "[RESPONSE][UAV"<< mMSG->getOrigem() <<"-STATUS-TO-U"<<selfID<<"] Status recebido: x:" << us.getLocationX();
            //cout << " y: " << us.getLocationY();
            //cout << " z: " << us.getLocationZ() << endl;
            ModuleMessage mm = castUAVMessageToModuleMessage(*mMSG);
            mm.setModule(2);

            msgs[selfID].push_back(mm);
        }else if(mMSG->getKind() == REQUEST_CONSENSUS && strcmp(mMSG->getName(), "collision") == 0){
            //cout << "minha coordenadas: " << position[mMSG->getOrigem()].getX() << "|" << position[mMSG->getOrigem()].getY() << "|" << position[mMSG->getOrigem()].getZ() << endl;
            //cout << "SAVIOOOO" << endl;
            //cout << "UAV" << mMSG->getOrigem() << " Quer: " << mMSG->getKind();
            //cout << " nome: " << mMSG->getName() << " de: " << selfID << " mesmo que " << mMSG->getDestino() << endl;
            //Collision c = mMSG->getCollision();
            //cout << "Collision- UAV" << c.getUAV().getID() << " escapar por: "
            //                                        << c.getUAVCase() << endl;
            //cout << "Nas coordenadas: " << c.getCoordinate().getX() << "|" << c.getCoordinate().getY() << "|" << c.getCoordinate().getZ() << endl;
            //cout << "Coordenadas reais: " << position[selfID].getX() << "|" << position[selfID].getY() << "|" << position[selfID].getZ() << endl;
            ModuleMessage mm = castUAVMessageToModuleMessage(*mMSG);
            mm.setModule(2);

            msgs[selfID].push_back(mm);
        }else if(mMSG->getKind() == TASK_WAITTING &&
                (strcmp(mMSG->getName(), "WAITTING") == 0
                        || strcmp(mMSG->getName(), "WAITTING-CHECK") == 0)){
            if(tasksVector[selfID][itera[selfID]].getStatus() == Task::STARTED){
                UAVMessage *sendMSGEvt = new UAVMessage("WAITTING-CHECK", TASK_WAITTING);
                sendMSGEvt->setOrigem(selfID);
                sendMSGEvt->setDestino(selfID);
                sendMSGEvt->setTask(mMSG->getTask());
                cout << "AAAAAA" << endl;
                scheduleAt(simTime()+2, sendMSGEvt);
                cout << "CCCC" << endl;
            }else{
                int leader = tasksVector[selfID][itera[selfID]].getLeader();
                if(qtdFormacao == 0 && leader < 0){
                    qtdFormacao = (leader*(-1));
                    cout << "PASSOU FORMAÇÃO PARA: " << qtdFormacao << endl;
                }
                Task t = mMSG->getTask();

                tasksVector[selfID][itera[selfID]].setLeader(leader+1);
                cout << tasksVector[selfID][itera[selfID]].getLeader() << endl;

                //TEM 2 CASOS...
                //O CASO QUE OS UAVS TERMINAM ANTES DO LIDER
                //O CASO QUE O LIDER TERMINA PELO MENOS ANTES DO ÚLTIMO UAV

                //IF O LIDER TERMINOU ANTES, ENTÃO ENTRA NESSE IF:
                if(tasksVector[selfID][itera[selfID]].getLeader() == -1){
                    cout << "Marquei atividade do lider como completa!" << endl;
                    //aqui se envia mensagem para todos os UAVs da tarefa
                    tasksVector[selfID][itera[selfID]].setStatus(Task::SIGNNED);
                    //itera[selfID]++;
                    //waypoints[selfID] = 0;
                    //ativo[selfID] = true;
                    //QWEQWEQW

                    UAVMessage *uavMSG = new UAVMessage("NEXT", TASK_COMPLETED);
                    uavMSG->setOrigem(selfID);
                    enviarMensagemParaTodosOsUAVs(uavMSG, qtdFormacao-1);
                    qtdFormacao = 0;

                    if(tasksVector[selfID].size() != itera[selfID]+1){
                        ativo[selfID] = true;
                    }
                }

                //PARA O CASO DO LIDER TERMINAR DEPOIS, O UAVMOBILITY DEVE TRATAR
            }
        }else if(mMSG->getKind() == TASK_COMPLETED && strcmp(mMSG->getName(), "NEXT") == 0){
            cout << selfID << " - RECEBI A MENSAGEM DE IR PARA A PROXIMA FORMAÇÃO" << endl;
            tasksVector[selfID][itera[selfID]].setStatus(Task::SIGNNED);
            //itera[selfID]++;
            //waypoints[selfID] = 0;
            if(tasksVector[selfID].size() != itera[selfID]+1){
                ativo[selfID] = true;
            }
        }else if(mMSG->getKind() == Message::TASK_MESSAGE
                && (strcmp(mMSG->getName(), "grp2down") == 0
                        || strcmp(mMSG->getName(), "grp2mid") == 0
                        || strcmp(mMSG->getName(), "grp2up") == 0)){
            cout << selfID << " - RECEBI grp2mid DE IR EM FORMAÇÃO [" << selfID << "]" << endl;
            Task t = mMSG->getTask();
            t.setUAV(UAV(selfID));
            Coord posUAV = this->pegarPosicaoUAV();
            Coord target = castCoordinateToCoord(t.getTarget()) + posUAV;
            cout << "TARGET QUE VEIO: " << castCoordinateToCoord(t.getTarget()) << "TARGET: " << target << " MINHA POS: " << posUAV << endl;
            t.setTarget(castCoordToCoordinate(target));
            tasksVector[selfID].push_back(t);
            //itera[selfID]++;
            if(tasksVector[selfID].size() != itera[selfID]+1){
                ativo[selfID] = true;
            }
        }
}

void ModuloComunicacao::handleNessagesBetweenModules(UAVMessage *mMSG){
    //SE MENSAGEM FOR CHECKING OU HOUVER MENSAGEM VINDA DO MOBILITY
        if(mMSG->getKind() == CHECKING_MESSAGE && strcmp(mMSG->getName(), "checking") == 0){

            for (int i = 0; i < msgs[selfID].size(); i++) {
                ModuleMessage mm = msgs[selfID][i];
                if(mm.getModule() == MODULE_ID){
                    //cout << "[MxM] " << mm.getMsg() << " | " << mm.getSource() << endl;
                    if(mm.getCode() == Message::TASK_MESSAGE && (
                            strcmp(mm.getMsg(), "grp2down") == 0
                            || strcmp(mm.getMsg(), "grp2mid") == 0
                            || strcmp(mm.getMsg(), "grp2up") == 0)){
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(), mm.getCode());
                        uavMSG->setOrigem(selfID);
                        uavMSG->setTask(mm.getTask());
                        //uavMSG->setDestino(i+1);
                        //send(uavMSG, "out", uavMSG->getDestino()-1);
                        cout << "LANÇOU " << mm.getMsg() << " para outros UAVs." << endl;

                        enviarMensagemParaTodosOsUAVs(uavMSG, 2);
                    }else if(strcmp(mm.getMsg(), "location") == 0 && mm.getCode() == REQUEST_POSITION_UAV){
                        //cout << "[MM] ESTÁ QUERENDO SABER LOCATION: " << mm.getSource() << endl;
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(), mm.getCode());
                        uavMSG->setOrigem(selfID);
                        //uavMSG->setDestino(i+1);
                        //send(uavMSG, "out", uavMSG->getDestino()-1);

                        enviarMensagemParaTodosOsUAVsAtivos(uavMSG);
                    }else if(strcmp(mm.getMsg(), "collision") == 0 && mm.getCode() == REQUEST_CONSENSUS){
                        //cout << "[MM] ESTÁ QUERENDO SABER CONSENSUS: " << mm.getSource() << endl;
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(), mm.getCode());
                        uavMSG->setOrigem(selfID);
                        uavMSG->setDestino(mm.getDestination());
                        uavMSG->setCollision(mm.getCollision());
                        if(uavMSG->getDestino() > selfID){
                            send(uavMSG, "out", uavMSG->getDestino()-1);
                        }else{
                            send(uavMSG, "out", uavMSG->getDestino());
                        }
                    }else if(strcmp(mm.getMsg(), "WAITTING") == 0 && mm.getCode() == TASK_WAITTING){
                        cout << "Mensagem de " << mm.getMsg() << " CODIGO: " << mm.getCode() << " DE: " << selfID << endl;
                        UAVMessage *uavMSG = new UAVMessage(mm.getMsg(), mm.getCode());
                        uavMSG->setOrigem(selfID);
                        uavMSG->setDestino(mm.getDestination());
                        uavMSG->setTask(mm.getTask());
                        send(uavMSG, "out", uavMSG->getDestino());
                    }

                    auto it = msgs[selfID].begin();
                    it = it + i;
                    //msgs[selfID].pop_back();
                    msgs[selfID].erase(it);

                }

            }

            UAVMessage *sendMSGEvt = new UAVMessage("checking", CHECKING_MESSAGE);
            sendMSGEvt->setOrigem(selfID);
            scheduleAt(simTime()+2, sendMSGEvt);
        }
}
