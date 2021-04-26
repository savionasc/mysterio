#include "StatusC1.h"
#include "../communication/CommunicationSocket.h"

StatusC1::StatusC1() { }

StatusC1::~StatusC1() { }

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
