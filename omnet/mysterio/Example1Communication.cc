#include <iostream>
#include "../communication/CommunicationSocket.h"
#include "Example1Communication.h"

using namespace std;

namespace mysterio {

Example1Communication::Example1Communication() { }

Example1Communication::~Example1Communication() { }

}

int main(int argc, char const *argv[]){
    CommunicationSocket comm;
    comm.listenSocket();
    std::cout << "Fim da execução" << std::endl;
    return 0;
}
