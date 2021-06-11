cplusplus {{
#include "mysterio/omnet/common/DroneStatus.h"
#include <string>
}}

class DroneStatus {
    @existingClass;
}

message DroneMessage extends cMessage {
    int origem = -1;
    int destino = -1;
    DroneStatus status;
    string titulo;
}

