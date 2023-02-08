cplusplus {{
    // *Mysterio framework observation*
    // Here includes external classes
    
    // #include "../../src/status/UAVStatus.h"
    // #include "../../src/mission/Task.h"
	#include <string>
}}


// *Mysterio framework observation*
// Here define types of classes to use internally in messages

// class UAVStatus {
//    @existingClass;
// }

// class Task {
//    @existingClass;
// }

message UAVMessage extends cMessage {
    // *Mysterio framework observation*
	// Variables defined using acceptable types in the OMNeT++ 
    int source = -1;
    int destination = -1;
    string title;
    
    // *Mysterio framework observation*
    // Example of variables defined using Mysterio classes.
    // UAVStatus status;
    // Task task;
}

