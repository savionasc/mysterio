#include <iostream>

#include "communication/CommunicationSocket.h"
using namespace std;

int main(int argc, char const *argv[]){
	std::cout << "Digite um valor" << std::endl;
	CommunicationSocket comm;
	while(1){
		int a;
		cin >> a;
		//comm.onMessageReceive(a);
		if(a == -1){
			cout << "Saindo!" << endl;
			break;
		}
	}
	return 0;
}