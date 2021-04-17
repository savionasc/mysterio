#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"
#include <iostream>

using namespace std;

StatusC1::StatusC1() { }

StatusC1::~StatusC1() { }

void StatusC1::enviarResposta(int resp){
    CommunicationSocket cs;
    Communicable src;
    Communicable dest; //UAV
    //dest. //dest.setID();
    cout << "Passando por status" << endl;
    //MSG
    cs.sendMessage(&src, &dest, 0);
}

/*void enviarResposta(char *resp){
   CommunicationSocket cs;
   Communicable dest; //UAV
   dest. //dest.setID();
   //MSG
   cs.sendMessage(this, dest, msg);
}*/
