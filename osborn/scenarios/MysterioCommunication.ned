package mysterio.osborn.scenarios;

import inet.node.inet.StandardHost;
import mysterio.osborn.uavs.ModuloComunicacao;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

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
        
    connections allowunconnected:
        for i = 0..numHosts-1 {
            //node[i].port[1] <--> node[i+1].port[0];
            UAV[i].out++ --> {  delay = 10ms; } --> UAV[i+1].in++;
        	UAV[i].in++ <-- {  delay = 10ms; } <-- UAV[i+1].out++;
        }
    //connections:
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[1].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[2].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[3].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[4].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[4].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[5].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[5].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[6].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[6].out++;
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[7].in++;
        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[7].out++;
        UAV[1].out++ --> {  delay = 10ms; } --> UAV[2].in++;
        UAV[1].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
        UAV[1].out++ --> {  delay = 10ms; } --> UAV[3].in++;
        UAV[1].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
        UAV[2].out++ --> {  delay = 10ms; } --> UAV[1].in++;
        UAV[2].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
        UAV[2].out++ --> {  delay = 10ms; } --> UAV[3].in++;
        UAV[2].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
        UAV[3].out++ --> {  delay = 10ms; } --> UAV[0].in++;
        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[0].out++;
        UAV[3].out++ --> {  delay = 10ms; } --> UAV[1].in++;
        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
        UAV[3].out++ --> {  delay = 10ms; } --> UAV[2].in++;
        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
}
