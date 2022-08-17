cplusplus {{
#include "../../src/status/UAVStatus.h"
#include "../../src/mission/Task.h"
#include "consensus/Collision.h"
#include <string>
}}

class UAVStatus {
    @existingClass;
}

class Task {
    @existingClass;
}

class Collision {
    @existingClass;
}

message UAVMessage extends cMessage {
    int origem = -1;
    int destino = -1;
    UAVStatus status;
    Task task;
    Collision collision;
    string titulo;
}

