#include <iostream>
#include "communication/Loko.h"

using namespace std;

int main(int argc, char const *argv[]){
	cout << "Digite um valor" << endl;
	Loko l;
	while(1){
		int a;
		cin >> a;
		l.onMessageReceive(a);
		if(a == -1){
			cout << "Saindo!" << endl;
			break;
		}
	}
	return 0;
}