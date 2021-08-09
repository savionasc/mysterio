#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
<<<<<<< HEAD
#include <string>
=======
#include <iostream>
#include <vector>
using namespace std;
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c

class Command {
public:
    Command(){string cmd = "go"; this->v.push_back(cmd);} //comand sendMessage
    virtual ~Command(){}
<<<<<<< HEAD

    int id = -1;
    std::string cmd = "";
    //Coordinate coord; defalt -1, -1, -1
=======
private:
    vector<string> v;
>>>>>>> fdd33f45c5f3bb8a8d8af53ef66c338281cdd69c
};

#endif
