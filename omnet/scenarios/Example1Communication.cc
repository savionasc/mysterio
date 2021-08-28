#include "../scenarios/Example1Communication.h"
#include <iostream>
#include "../communication/CommunicationSocket.h"
#include "../Singleton.h"

using namespace std;

int main(int argc, char const *argv[]){
    //Singleton* singleton = Singleton::GetInstance("BAR");
    //std::cout << singleton->value() << "\n";

	//This number identification will be important to set database execution
	int numExecution = 1; 

    CommunicationSocket comm;
    comm.listenSocket();
    std::cout << "Fim da execução" << std::endl;
    return 0;
}
