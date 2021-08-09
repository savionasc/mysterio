#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Command {
public:
    Command(){string cmd = "go"; /*this->v.push_back(cmd);*/}
    virtual ~Command(){}

    int id = -1;
    std::string cmd = "";
    //Coordinate coord; defalt -1, -1, -1
    //vector<string> v;
};

#endif
