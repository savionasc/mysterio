package mysterio.marko.scenarios;

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
//module ModuloComunicacao extends StandardHost {
//    parameters:
//        @class(ModuloComunicacao);
//    gates:
//        input in[];
//        output out[];
//}