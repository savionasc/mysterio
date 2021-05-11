#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"

//Adicionar isso?
//Inicio
void StatusC1::saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *status){
    Coordinate coord;

    coord.setX(x);
    coord.setY(y);
    coord.setZ(z);
    status->setUAVLocation(coord, idUAV);
}

void StatusC1::saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *status){
    status->setUAVLocation(coord, idUAV);
}

Coordinate StatusC1::requestUAVCurrentPosition(int idUAV, StatusC1 *status){
    return status->getUAVLocation(idUAV);
}

void StatusC1::saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *status){
    status->setUAVVelocity(velocity, idUAV);
}

double StatusC1::requestUAVCurrentVelocity(int idUAV, StatusC1 *status){
    return status->getUAVVelocity(idUAV);
}
//Fim

void StatusC1::enviarResposta(int resp){ //Ou reformula essa função ou apaga //Ela não envia pra ninguém
    if(resp > 15 || resp == 1){

    }

    CommunicationSocket cs;
    Communicable src;
    Communicable dest; //UAV
    //dest. //dest.setID();
    cout << "Passando por status" << endl;
    Message m("Resposta", 15);
    cs.sendMessage(&src, &dest, m);
}
