#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Command {
public:
    Command(){ this->instruction = "go"; /*this->v.push_back(cmd);*/}
    virtual ~Command(){}

    int id = -1;
    std::string instruction = "";
    //Criar um toString no coordinate
    //Depois criar uma função que retorna coordinate recebendo uma string
    //Coordinate coord; defalt -1, -1, -1

    vector<string> args; //queue?
};

#endif
