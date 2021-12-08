package mysterio.octopus.scenarios;

import inet.node.base.ApplicationLayerNodeBase;
import inet.node.inet.StandardHost;

//Estatico
module BasestationUAVs extends ApplicationLayerNodeBase {
    parameters:
        @display("i=block/broadcast");
}

//Estatico
//module Sheep extends ApplicationLayerNodeBase {
//    parameters:
//        @display("i=block/broadcast");
//}



//simple MysterioMobility extends MovingMobilityBase
module Sheep extends StandardHost {
    parameters:
        @display("i=block/broadcast");
        @class(Sheep);
    //gates:
        //input in;
        //output out;
}