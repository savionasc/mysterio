#include "CommunicationSocket.h"

#include "../../../src/mysterio/Mysterio.h"
#include <string.h>

//Enviar mensagens Unicast, Broadcast e Multicast

//Mostrar opções sincronas e assincronas / Alguma forma de Enviar mensagens

//Ou criar um outra mensagem e Minha mensagem herdar dessa classe, ou mudar as coisas
//#include "../../common/msg/MinhaMensagem_m.h"

//typedef std::map<int, MinhaMensagem> MyMap;

//Sockets
#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;

int conexoes[5], ct = -1;

//bool conectarNovoClient(int);
//bool esperarMensagem(int);
//bool enviarResposta(int);


class socket_conectar {
public:
    void operator()(int param)
    {
        CommunicationSocket cs;
        while(cs.conectarNovoClient(param)){ }
    }
};

class socket_receber {
public:
    void operator()(int param)
    {
        CommunicationSocket cs;
        while(cs.esperarMensagem(param)){ }
    }
};

class socket_enviar {
public:
    void operator()(int param)
    {
        CommunicationSocket cs;
        while(cs.enviarResposta(param)){ }
    }
};

bool CommunicationSocket::esperarMensagem(int newSd){
    //buffer to send and receive messages with
    char msg[1500];
    //receive a message from the client (listen)

    //cout << "Awaiting client response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    recv(newSd, (char*)&msg, sizeof(msg), 0);
    if(!strcmp(msg, "exit")){
        std::cout << "Client has quit the session" << std::endl;
        return false;
    }else if(!strcmp(msg, "status")){
        StatusC1 status;
        status.onMessageReceive(1);
    }
    std::cout << "Client: " << msg << std::endl;
    return true;
}

bool CommunicationSocket::enviarResposta(int newSd){
    char msg[1500];
    std::cout << ">";
    std::string data;
    cin >> data; //Dessa forma só não aceita espaços
    //cin.getline(msg,sizeof(msg));
    //getline(cin, data);
    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit"){
        //send to the client that server has closed the connection
        send(newSd, (char*)&msg, strlen(msg), 0);
        return false;
        //break;
    }
    //send the message to client
    send(newSd, (char*)&msg, strlen(msg), 0);
    return false; //true; //False só pra parar
}

bool CommunicationSocket::conectarNovoClient(int serverSd){
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cerr << "Error accepting request from client!" << std::endl;
        exit(1);
        return false;
    }
    std::cout << "Connected with client!" << std::endl;
    ct++;
    conexoes[ct] = newSd;
    thread conectar(socket_conectar(), serverSd);
    thread receber(socket_receber(), conexoes[ct]);
    receber.join();
    return true;
}

int CommunicationSocket::configurar(int port){
    if(port < 1000){
        cerr << "Usage: port" << std::endl;
        exit(0);
    }

    //int port = atoi(argv[1]);
    char msg[1500];

    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Error establishing the server socket" << std::endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
        sizeof(servAddr));
    if(bindStatus < 0){
        cerr << "Error binding socket to local address" << std::endl;
        exit(0);
    }
    std::cout << "Waiting for a client to connect..." << std::endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    return serverSd;
}

void CommunicationSocket::envMensagem(){
    std::cout << "Digite um numero para escolher quem vai receber" << std::endl;
    while(true){
        int id;
        cin >> id;
        //scanf("%d", &id);
        if(id == -1){ //Broadcast
            for (int i = 0; i <= ct; i++){
                thread enviar(socket_enviar(), conexoes[i]);
                enviar.join();
            }
        }else{ //unicast
            thread enviar(socket_enviar(), conexoes[id]);
            enviar.join();
        }
        //enviar.joinable();
        //cout << "Digite o que você quer enviar" << std::endl;
    }
}

void CommunicationSocket::listening(){

    int serverSd = configurar(1111);
    thread conectar(socket_conectar(), serverSd);
    while(ct == -1){

    }
    cout << "Passando por Socket Communication" << endl;
    int antigo = 0;
    if(antigo == 0){
        while(1){
            thread enviar(socket_enviar(), conexoes[0]);
            enviar.join();
        }
    }else{
        envMensagem();
    }

    conectar.join();
}
//Fim sockets


void CommunicationSocket::sendMessage(Communicable *source, Communicable *dest, int msg){
    if(msg == 0){
        dest->onMessageReceive(msg);
        //Loko* lok = dynamic_cast<Loko*>(dest);
        //lok->onMessageReceive(msg);
    }
}

void CommunicationSocket::testeMessage(){

}

CommunicationSocket::CommunicationSocket() { }

CommunicationSocket::~CommunicationSocket() { }

/*void Comm::sendMessageDroneToDrone(int idSource, int idDestination, MinhaMensagem *msg){
    messages[idDestination] = *msg;
}*/

bool CommunicationSocket::hasMessageToDrone(int destination){
    //std::map<int,MinhaMensagem>::iterator it = messages.find(destination);
    //if(it->first == destination)
    //if(it != messages.end())
    if(1)
        return true;
    else
        return false;
}

int CommunicationSocket::hasMessages(){
    return 1;//messages.size();
}

std::queue<int> CommunicationSocket::messagesToSend(){
    std::queue<int> fila;
    //for (std::map<int, MinhaMensagem>::iterator it=messages.begin(); it!=messages.end(); ++it)
        //fila.push(it->first);
    return fila;
}

/*MinhaMensagem Comm::receiveMessage(int destination){
    return messages[destination];
}*/

void CommunicationSocket::markAsReceived(int destination){
    //messages.erase(destination);
}

void CommunicationSocket::connectANewUAV(int ID, StatusC1 *aggregator){
    UAV newUAV;
    newUAV.setID(ID);
    aggregator->addUAV(newUAV);
}

void CommunicationSocket::disconnectUAV(int ID, StatusC1 *aggregator){
    aggregator->removeUAV(ID);
}

void CommunicationSocket::saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *aggregator){
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    coord.z = z;
    aggregator->setUAVLocation(coord, idUAV);
}

void CommunicationSocket::saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate CommunicationSocket::requestUAVCurrentPosition(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void CommunicationSocket::saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double CommunicationSocket::requestUAVCurrentVelocity(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}

/*void Communication::setNumNodes(int nodes, Aggregator* aggregator){
    aggregator->numNodes = nodes;
}

int Communication::getNumNodes(Aggregator* aggregator){
    return aggregator->numNodes;
}*/
