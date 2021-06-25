cplusplus {{
#include "mysterio/omnet/common/DroneStatus.h"
#include "../../src/mission/Task.h"
#include <string>
}}

class DroneStatus {
    @existingClass;
}

class Task {
    @existingClass;
}

message DroneMessage extends cMessage {
    int origem = -1;
    int destino = -1;
    DroneStatus status;
    Task task;
    string titulo;
}

