package mysterio.quentin.scenarios;

import inet.node.inet.StandardHost;
import mysterio.quentin.uavs.divided.ModuloComunicacaoAlpha;
import mysterio.quentin.uavs.divided.ModuloComunicacaoBeta;

import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

module BasestationUAVs extends ApplicationLayerNodeBase {
    parameters:
        @display("i=block/broadcast");
}

network MysterioCommunicationUnified {
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
        
        UAV: ModuloComunicacaoAlpha {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
        
        UAV2: ModuloComunicacaoBeta {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
//    connections:
//        UAV[0].out++ --> {  delay = 100ms; } --> UAV[1].in++;
//        UAV[0].in++ <-- {  delay = 100ms; } <-- UAV[1].out++;
}
