package mysterio.connors.scenarios;

import inet.node.inet.StandardHost;
import mysterio.connors.uavs.example2.ModuloComunicacao2;

import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

module BasestationUAVs extends ApplicationLayerNodeBase {
    parameters:
        @display("i=block/broadcast");
}

network MysterioCommunication1 {
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
        
        UAV[numHosts]: ModuloComunicacao2 {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
        CAR: BasestationUAVs {
            @display("p=300,420");
        }
    connections:
        UAV[0].out++ --> {  delay = 100ms; } --> UAV[1].in++;
        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[1].out++;
}
