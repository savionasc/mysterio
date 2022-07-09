package mysterio.osborn.scenarios;

import inet.node.inet.StandardHost;
import mysterio.osborn.uavs.ModuloComunicacao;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

network MysterioCommunication {
    parameters:
        int numHosts;
        @display("bgb=1500,1500");
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
        
//    connections allowunconnected:
//        for i = 0..numHosts-1 {
//            //node[i].port[1] <--> node[i+1].port[0];
//            UAV[i].out++ --> {  delay = 10ms; } --> UAV[i+1].in++;
//        	UAV[i].in++ <-- {  delay = 10ms; } <-- UAV[i+1].out++;
//        }
  
//  connections allowunconnected:
//    for i=0..numHosts-1, for j=0..numHosts-1 {
//        UAV[i].out++ --> {  delay = 10ms; } --> UAV[j].in++ if i!=j;
//        UAV[i].in++ <-- {  delay = 10ms; } <-- UAV[j].out++ if i!=j;
//    }
    
//  connections allowunconnected:
//    for i=0..numHosts-1{
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=0;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[i].out++ if i!=0;
//
//    }

//    connections allowunconnected:
//        for i = 0..numHosts-1 {
//	        for j = 0..numHosts-1 {
//    	        UAV[i].out++ --> {  delay = 10ms; } --> UAV[j].in++;
//        		UAV[i].in++ <-- {  delay = 10ms; } <-- UAV[j].out++;
//        	}
//        }

  connections allowunconnected:
    for i=0..numHosts-1{
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=0;
        UAV[1].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=1;
        UAV[2].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=2;
        UAV[3].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=3;
        UAV[4].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=4;
        UAV[5].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=5;
        UAV[6].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=6;
        UAV[7].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=7;
        UAV[8].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=8;
        UAV[9].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=9;

    }

//    connections:
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[1].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[2].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[3].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[4].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[4].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[5].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[5].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[6].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[6].out++;
//        UAV[0].out++ --> {  delay = 10ms; } --> UAV[7].in++;
//        UAV[0].in++ <-- {  delay = 10ms; } <-- UAV[7].out++;
//        UAV[1].out++ --> {  delay = 10ms; } --> UAV[2].in++;
//        UAV[1].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
//        UAV[1].out++ --> {  delay = 10ms; } --> UAV[3].in++;
//        UAV[1].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
//        UAV[2].out++ --> {  delay = 10ms; } --> UAV[1].in++;
//        UAV[2].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
//        UAV[2].out++ --> {  delay = 10ms; } --> UAV[3].in++;
//        UAV[2].in++ <-- {  delay = 10ms; } <-- UAV[3].out++;
//        UAV[3].out++ --> {  delay = 10ms; } --> UAV[0].in++;
//        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[0].out++;
//        UAV[3].out++ --> {  delay = 10ms; } --> UAV[1].in++;
//        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[1].out++;
//        UAV[3].out++ --> {  delay = 10ms; } --> UAV[2].in++;
//        UAV[3].in++ <-- {  delay = 10ms; } <-- UAV[2].out++;
}
