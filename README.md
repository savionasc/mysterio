# Mysterio 👾
#### Mysterio Framework Planning Guide

To build an instance for your application using this framework, we indicate our architecture which can be seen in the following figure:

![Mysterio Framework and its components](https://github.com/savionasc/mysterio/blob/main/UAV%20architecture-Componente.png)

<p style="text-align: center;">Mysterio Framework Software Architecture</p>

## Recommendations for building Multi-UAV systems using our architecture and the Mysterio Framework:

Beforehand, the base code of the Mysterio framework is present in the [src](https://github.com/savionasc/mysterio/tree/main/src) folder and the [connors](https://github.com/savionasc/mysterio/tree/main/connors), [electro](https://github.com/savionasc/mysterio/tree/main/electro) and [marko](https://github.com/savionasc/mysterio/tree/main/marko) folders are instances of the framework.

#### UAVs and [UAVCommunication](https://github.com/savionasc/mysterio/blob/main/src/communication/UAVCommunication.h)
For UAVs, whether virtual or real, the UAVs' internal management software has to be your way (user-developed) so that it uses UAVCommunication as the communication interface between the framework and the UAVs. It is important to make it clear that the framework must be present in your base station (command center), that is, a separate piece of software from UAVs. In UAVs, the communication with the framework takes place through the UAVCommunication, which is the communication interface. It has by default methods that allow connection and disconnection with the framework, as well as sending and receiving messages.

#### [Communication](https://github.com/savionasc/mysterio/blob/main/src/communication/Communicable.h) e [Communicable](https://github.com/savionasc/mysterio/blob/main/src/communication/Communicable.h) 
The component Communicator UAVs is responsible for making all and any communication between the framework and the UAVs. The implementation of this component was separated into 3 classes, one of the classes that is present in UAVs (UAVCommunication) and the others not mentioned (Communication and Communicable) that are present in the framework. In UAVs, the UAVCommunication class is responsible for communicating between the UAV and the framework and this component must use the same communication protocol as the framework to communicate. On the base station side, when the Communication component receives messages, it forwards them to other components that deal with each specific type of information, such as the Status Manager or Task Managar which both extend from the Communicable class. It is worth mentioning that every manager component extends the Communicable class and thus reuses its methods to receive the information from the UAVs that this component is responsible for.

#### [StatusManager](https://github.com/savionasc/mysterio/blob/main/src/status/StatusManager.h)

Component responsible for the status (properties, attributes and characteristics) of UAVs.

The ideal is to implement, in this framework component, methods referring to each UAVs Status property. Depending on the user scenario, a Multi-UAV System has a lot of data that must be passed from the UAV to the Framework. Here are some of the Status data that might be relevant:

> Note: It is important to understand that when dealing with Heterogeneous UAVs, you may have some UAV that does not support all the listed properties/features, in addition to including new ones.

UAVs Status Data:
* Battery
* Flight time
* Altitude and Location/geographical position
* Velocity
* Payload
* Availability (communication or tasks)
* idleness

> Use a status class, where each information considered as Status must become an attribute of the class or derive new classes that represent different attributes each.

In the StatusManager there must be at least one method for each Status, where the component will pass the status of the UAV(s) to other components.

#### [Repository](https://github.com/savionasc/mysterio/blob/main/src/database/Repository.h)
Data persistence component, whether UAVs Status, task and mission data or other information, this is the framework component that is responsible for saving the data in the user database. The ideal in the framework instance is to know how to deal with all the information it receives from the uavs and decide whether or not it is relevant to be saved.

#### [Task](https://github.com/savionasc/mysterio/blob/main/src/mission/Task.h)
Class that represents the tasks, which can be approached in at least two different ways. The first would be not to directly specify how to perform the task, but just indicate through attributes how to differentiate that task and what it represents, so that the UAV must already interpret and perform pre-programmed actions. The second way of working with tasks would be to pass to the task the commands that the UAV must execute to perform that task.

#### [TaskManager](https://github.com/savionasc/mysterio/blob/main/src/taskmanager/TaskManager.h)
Component responsible for managing and receiving task information.

#### [MissionPlanner](https://github.com/savionasc/mysterio/blob/main/src/mission/MissionPlanner.h)
Component that has full mission management autonomy, which is composed of a set of tasks assigned to UAVs. This component already has its own implementation that does not require the user's own implementation.