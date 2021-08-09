#ifndef MYSTERIO_SRC_MISSION_COMMAND_H_
#define MYSTERIO_SRC_MISSION_COMMAND_H_
#include <string>

class Command {
public:
    Command(){}
    virtual ~Command(){}

    int id = -1;
    std::string cmd = "";
    //Coordinate coord; defalt -1, -1, -1
};

#endif
