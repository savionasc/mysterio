package mysterio.structure.uavs;

import inet.node.inet.StandardHost;
import mysterio.structure.uavs.CommunicationModule;
//import "../uavs/CommunicationModule.h";

module CommunicationModule extends StandardHost {
    parameters:
        @class(CommunicationModule);
}