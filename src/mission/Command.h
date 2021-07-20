#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <iostream>
#include <vector>
using namespace std;

class Command {
public:
    Command(){string cmd = "go"; this->v.push_back(cmd);} //comand sendMessage
    virtual ~Command(){}
private:
    vector<string> v;
};

#endif
