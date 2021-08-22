#include "../scenarios/Example1Communication.h"
#include <iostream>
#include "../communication/CommunicationSocket.h"

using namespace std;

int main(int argc, char const *argv[]){

	//This number identification will be important to set database execution
	int numExecution = 1; 

    CommunicationSocket comm;
    ///
    ///
    comm.listenSocket();
    std::cout << "Fim da execução" << std::endl;
    return 0;
}
