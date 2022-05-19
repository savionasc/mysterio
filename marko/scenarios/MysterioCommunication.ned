package mysterio.marko.scenarios;

import inet.node.inet.StandardHost;
import mysterio.marko.uavs.ModuloComunicacao;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;
import mysterio.marko.sheep.Sheep;

network MysterioCommunication {
    parameters:
        int numHosts;
        @display("bgb=1000,1000");
    submodules:
        visualizer: IntegratedVisualizer {
            @display("p=100,50");
        }
        canvasVisualizer: IntegratedCanvasVisualizer {
            @display("p=100,50");
        }
        
        host[numHosts]: ModuloComunicacao {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
        basestation: BasestationUAVs {
            @display("p=300,420");
        }
        sheep: Sheep {
            @display("p=550,220");
        }
        
    //connections allowunconnected:
    connections:
        host[0].out++ --> {  delay = 100ms; } --> sheep.in++;
        host[0].out++ --> {  delay = 100ms; } --> host[1].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[1].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[2].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[2].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[3].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[3].out++;
        host[0].out++ --> {  delay = 100ms; } --> host[4].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[4].out++;
}
