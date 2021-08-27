#include "../scenarios/Example1Communication.h"
#include <iostream>
#include "../communication/CommunicationSocket.h"
#include "../singleton.cc"

using namespace std;


Singleton* Singleton::pinstance_{nullptr};
std::mutex Singleton::mutex_;

Singleton *Singleton::GetInstance(const std::string& value){
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr){
        pinstance_ = new Singleton(value);
    }
    return pinstance_;
}

void ThreadFoo(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}

int main(int argc, char const *argv[]){

    //std::cout <<"If you see the same value, then singleton was reused (yay!\n" <<
                //"If you see different values, then 2 singletons were created (booo!!)\n\n" <<
                //"RESULT:\n";
    //std::thread t1(ThreadFoo);
    //std::thread t2(ThreadBar);
    //t1.join();
    //t2.join();

	//This number identification will be important to set database execution
	int numExecution = 1; 

    CommunicationSocket comm;
    ///
    ///
    comm.listenSocket();
    std::cout << "Fim da execução" << std::endl;
    return 0;
}
