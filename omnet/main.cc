#include <iostream>

#include "communication/CommunicationSocket.h"
using namespace std;

int main(int argc, char const *argv[]){
	CommunicationSocket comm;
	comm.listening();
	std::cout << "AAAA" << std::endl;
	/*while(1){
	    std::cout << "Digite um valor" << std::endl;
		int a;
		cin >> a;
		//comm.onMessageReceive(a);
		if(a == -1){
			cout << "Saindo!" << endl;
			break;
		}
	}*/
	return 0;
}
