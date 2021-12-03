package mysterio.osborn.scenarios;

import inet.node.base.ApplicationLayerNodeBase;

module BasestationUAVs extends ApplicationLayerNodeBase {
    parameters:
        @display("i=block/broadcast");
}

module Sheep extends ApplicationLayerNodeBase {
    parameters:
        @display("i=block/broadcast");
}

//import inet.node.inet.StandardHost;

//simple MysterioMobility extends MovingMobilityBase
//module ModuloComunicacao2 extends StandardHost {
//    parameters:
//        @class(ModuloComunicacao2);
//    gates:
//        input in[];
//        output out[];
//}