package mysterio.octopus.sheep;
import inet.node.inet.StandardHost;

//Estatico import inet.node.base.ApplicationLayerNodeBase;
//module Sheep extends ApplicationLayerNodeBase {

//simple MysterioMobility extends MovingMobilityBase
module Sheep extends StandardHost {
    parameters:
        @display("i=block/broadcast");
        @class(Sheep);
    //gates:
        //input in;
        //output out;
}