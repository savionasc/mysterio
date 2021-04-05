cplusplus {{
#include "mysterio/samples/communication/msg/Status.h"
#include <string>
}}

class Status {
    @existingClass;
}

message MinhaMensagem extends cMessage {
    int origem = -1;
    int destino = -1;
    Status status;
    string titulo;
}

