#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"

//Adicionar isso?
//Inicio
void StatusC1::saveUAVCurrentPosition(int idUAV, double x, double y, double z, StatusC1 *aggregator){
    Coordinate coord;

    coord.setX(x);
    coord.setY(y);
    coord.setZ(z);
    aggregator->setUAVLocation(coord, idUAV);
}

void StatusC1::saveUAVCurrentPosition(int idUAV, Coordinate coord, StatusC1 *aggregator){
    aggregator->setUAVLocation(coord, idUAV);
}

Coordinate StatusC1::requestUAVCurrentPosition(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVLocation(idUAV);
}

void StatusC1::saveUAVCurrentVelocity(int idUAV, double velocity, StatusC1 *aggregator){
    aggregator->setUAVVelocity(velocity, idUAV);
}

double StatusC1::requestUAVCurrentVelocity(int idUAV, StatusC1 *aggregator){
    return aggregator->getUAVVelocity(idUAV);
}
//Fim

void StatusC1::enviarResposta(int resp){
    CommunicationSocket cs;
    Communicable src;
    Communicable dest; //UAV
    //dest. //dest.setID();
    cout << "Passando por status" << endl;
    Message m;
    cs.sendMessage(&src, &dest, m);
}

/*void enviarResposta(char *resp){
   CommunicationSocket cs;
   Communicable dest; //UAV
   dest. //dest.setID();
   //MSG
   cs.sendMessage(this, dest, msg);
}*/
