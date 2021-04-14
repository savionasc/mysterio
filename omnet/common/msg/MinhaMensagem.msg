cplusplus {{
#include "mysterio/omnet/common/msg/StatusModule.h"
#include <string>
}}

class StatusModule {
    @existingClass;
}

message MinhaMensagem extends cMessage {
    int origem = -1;
    int destino = -1;
    StatusModule status;
    string titulo;
}

