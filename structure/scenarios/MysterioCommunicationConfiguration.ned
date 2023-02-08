package mysterio.structure.scenarios;
import inet.node.inet.StandardHost;
import mysterio.structure.uavs.CommunicationModule;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.base.ApplicationLayerNodeBase;

network MysterioCommunicationConfiguration {
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
        
        UAV[numHosts]: CommunicationModule {    
            @display("i=misc/node_vs");
            @display("t=UAV");
        }
        
  connections allowunconnected:
    for i=0..numHosts-1{
        UAV[0].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=0;
        UAV[1].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=1;
        UAV[2].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=2;
        UAV[3].out++ --> {  delay = 10ms; } --> UAV[i].in++ if i!=3;
    }
}
