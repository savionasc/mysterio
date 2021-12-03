<<<<<<< HEAD:osborn/scenarios/MysterioCommunication1.ned
package mysterio.osborn.scenarios;

import inet.node.inet.StandardHost;
import mysterio.osborn.uavs.example2.ModuloComunicacao2;
=======
package mysterio.electro.scenarios;

import inet.node.inet.StandardHost;
import mysterio.electro.uavs.example2.ModuloComunicacao2;
>>>>>>> d56a638d1e7aadeab92e97cb76bc60a983ab954a:connors/scenarios/MysterioCommunication1.ned
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
        
        host[numHosts]: ModuloComunicacao2 {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
        basestation: BasestationUAVs {
            @display("p=300,420");
        }
    connections:
        host[0].out++ --> {  delay = 100ms; } --> host[1].in++;
        host[0].in++ <-- {  delay = 100ms; } <-- host[1].out++;
        //host[0].out++ --> {  delay = 100ms; } --> host[2].in++;
        //host[0].in++ <-- {  delay = 100ms; } <-- host[2].out++;
        //host[0].out++ --> {  delay = 100ms; } --> host[3].in++;
        //host[0].in++ <-- {  delay = 100ms; } <-- host[3].out++;
        //host[0].out++ --> {  delay = 100ms; } --> host[4].in++;
        //host[0].in++ <-- {  delay = 100ms; } <-- host[4].out++;
}
