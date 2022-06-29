#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <iostream>
#include <string.h>
#include <vector>

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

    void addNewArgument(char *arg){
        if(strlen(arguments) != 0){
            strcat (arguments,"|");
        }
        strcat (arguments,arg);
    }

    void getNextArgument(){
    }

    char* getArgument(int i){
        std::vector<char*> v;
        char* chars_array = strtok(arguments, "|");
        int a = -1;
        while(chars_array){
            if(++a == i){
                return chars_array;
            }
            //cout << chars_array << endl;
            v.push_back(chars_array);
            chars_array = strtok(NULL, "|");
        }
        return chars_array;
    }

private:
    //Add anothers attributies in this classes
    int ID;
    //instructions like "go" or "ComebackToBaseStation"
    char instruction[15];

    //Para os args,
    //Criar um toString no coordinate
    //Depois criar uma função que retorna coordinate recebendo uma string
    //Coordinate coord; default -1, -1, -1

    char arguments[50];
};

#endif
