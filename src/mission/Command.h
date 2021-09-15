#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
//#include <string>
#include <iostream>
//#include <vector>
using namespace std;

class Command {
public:
    int id = -1;
    char instruction[35];
    //std::string instruction = "go";
    //Criar um toString no coordinate
    //Depois criar uma função que retorna coordinate recebendo uma string
    //Coordinate coord; defalt -1, -1, -1

    //vector<string> args; //queue?
    char args[5][35];
};

#endif
