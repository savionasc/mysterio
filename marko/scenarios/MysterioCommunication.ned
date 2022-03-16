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
        
        UAV[numHosts]: ModuloComunicacao {    
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
        UAV[0].out++ --> {  delay = 100ms; } --> sheep.in++;
        UAV[0].out++ --> {  delay = 100ms; } --> UAV[1].in++;
        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[1].out++;
        UAV[0].out++ --> {  delay = 100ms; } --> UAV[2].in++;
        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[2].out++;
        UAV[0].out++ --> {  delay = 100ms; } --> UAV[3].in++;
        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[3].out++;
        UAV[0].out++ --> {  delay = 100ms; } --> UAV[4].in++;
        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[4].out++;
}
