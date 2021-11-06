cplusplus {{
#include "../../src/status/UAVStatus.h"
#include "../../src/mission/Task.h"
#include <string>
}}

class UAVStatus {
    @existingClass;
}

class Task {
    @existingClass;
}

message UAVMessage extends cMessage {
    int origem = -1;
    int destino = -1;
    UAVStatus status;
    Task task;
    string titulo;
}

