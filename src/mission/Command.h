#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <iostream>

using namespace std;

class Command {
public:
    int getId() {
        return ID;
    }

    void setId(int id) {
        ID = id;
    }

    const char* getInstruction() {
        return instruction;
    }

    const char* getArguments() {
        return arguments;
    }

private:
    int ID;
    char instruction[15]; //instructions like "go"

    //Para os args,
    //Criar um toString no coordinate
    //Depois criar uma função que retorna coordinate recebendo uma string
    //Coordinate coord; defalt -1, -1, -1
    char arguments[50];
};

#endif
