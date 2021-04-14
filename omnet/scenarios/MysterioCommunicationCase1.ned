package mysterio.omnet.scenarios;

import inet.node.inet.StandardHost;
import mysterio.omnet.ExComm1.uavs.ModuloComunicacaoCase1;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

network MysterioCommunicationCase1 {
    parameters:
        int numHosts;
        @display("bgb=1000,1000");
    submodules:
        visualizer: IntegratedVisualizer {
            @display("p=100,50");
        }
        host[numHosts]: ModuloComunicacaoCase1 {
            @display("i=misc/node_vs");
        }
    connections:
        host[0].out++ --> {  delay = 100ms; } --> host[1].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[1].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[2].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[2].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[3].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[3].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[4].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[4].out++;
}
