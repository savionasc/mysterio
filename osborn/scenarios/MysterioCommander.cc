#include "../../osborn/scenarios/MysterioCommander.h"

#include <iostream>

#include "../communication/UAVRegistry.cc"
#include "../communication/MysCommunication.h"
#include "../../src/database/RepositoryBase.h"
#include "../../src/mission/MissionPlanner.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../../src/mission/Command.h"
#include "../mission/Formation.h"

using namespace std;

void assignPreprogrammedTasks(int, MysStatusManager*, MysCommunication);

void testee(Message *msg){
    cout << "Esta é a mensagem: " << msg->getMsg() << endl;
    cout << "Destino: " << msg->getDestination() << endl;
}

void listenCommunication(){
    //Here starts the server communication

    int numeroDeUAVsEsperados = 5;
    MysStatusManager *ms;

    MysCommunication comm;
    //Port number
    int serverSocket = comm.configureSocketServer(1111);
    comm.setPortServer(serverSocket);
    if(serverSocket > 0){
        thread conectar = comm.listenForNewConnections();
        std::cout << "Waiting for a UAV to connect..." << std::endl;
        //waiting for the first UAV
        while(ms->getSize() < numeroDeUAVsEsperados){
        }

        assignPreprogrammedTasks(numeroDeUAVsEsperados, ms, comm);

        //Menu
        while(true){
            std::cout << "Select a UAV by id:" << std::endl;
            int id;
            std::cin >> id;
            TaskManager t;
            UAV u(id);
            cout << "Number of tasks assigned to selected UAV: " << t.getNumTasks(u) << endl;
            std::cout << "Type a message:" << std::endl;
            char m[1500];
            std::cin >> m;
            Message msg(m, Task::GOTO, -1, id);

            //Tratando mensagens ao digitar
            if(!strcmp(msg.getMsg(), "task")){
                TaskManager t;
                UAV u(0);
                std::cout << "Number of UAV tasks: " << t.getNumTasks(u) << "\n";
            }

            if(!strcmp(msg.getMsg(), "tasks")){
                TaskManager t;
                UAV u(id);
                std::cout << "UAV tasks: " << t.getNumTasks(u) << "\n";
                vector<Task> ts = t.getTaskList(u);
                for (int i = 0; i < t.getNumTasks(u); i++) {
                    cout << "Task: " << ts[i].getType() << endl;
                    cout << "Task status: " << ts[i].getStatus();
                    cout << "Task ID: " << ts[i].getID() << endl;
                }
            }

            cout << "Antes " << endl;
            if(!strcmp(msg.getMsg(), "three")){
                Formation fUAVs(3);
                /*Coordinate coord1(200.0,200.0,70.0);
                Coordinate coord2(400.0,400.0,70.0);
                Coordinate coord3(600.0,600.0,70.0);
                fUAVs.addPosition(coord1);
                fUAVs.addPosition(coord2);
                fUAVs.addPosition(coord3);*/
                fUAVs.addPosition(Coordinate(200.0,200.0,70.0));
                fUAVs.addPosition(Coordinate(400.0,400.0,70.0));
                fUAVs.addPosition(Coordinate(600.0,600.0,70.0));
                for (int i = 0; i < fUAVs.getNumberOfUAVs(); i++) {
                    //FALTA SÓ ALTOMATIZAR ESTE FOR


                    UAV u(i);
                    Task gotoc(u, fUAVs.getPosition(i));
                    gotoc.setType(Task::GOTO);
                    TaskManager t;
                    t.addTask(gotoc);


                    //OU AUTOMATIZAR ESTE ENVIO DE MENSAGENS QUE ESTÁ REPETITIVO DEMAIS...
                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    taskMessage.setDestination(i);
                    taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                    comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
                }

            }else if(!strcmp(msg.getMsg(), "square")){
                Formation fUAVs(4);
                fUAVs.addPosition(Coordinate(250.0,250.0,70.0));
                fUAVs.addPosition(Coordinate(250.0,500.0,70.0));
                fUAVs.addPosition(Coordinate(500.0,250.0,70.0));
                fUAVs.addPosition(Coordinate(500.0,500.0,70.0));

                for (int i = 0; i < fUAVs.getNumberOfUAVs(); i++) {
                    //FALTA SÓ ALTOMATIZAR ESTE FOR

                    UAV u(i);
                    Task gotoc(u, fUAVs.getPosition(i));
                    gotoc.setType(Task::GOTO);
                    TaskManager t;
                    t.addTask(gotoc);


                    //OU AUTOMATIZAR ESTE ENVIO DE MENSAGENS QUE ESTÁ REPETITIVO DEMAIS...
                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    taskMessage.setDestination(i);
                    taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                    comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
                }
            }else if(!strcmp(msg.getMsg(), "goto")){
                cout << "Entrou GOTO" << endl;
                //take off
                Coordinate currentP(0.0,100.0,70.0);
                UAV u(id);
                Task gotoc(u, currentP);
                //gotoc.setType(Task::GOTO);

                gotoc.setType(Task::GOTO);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);

            }else if(!strcmp(msg.getMsg(), "pos1")){
                testee(&msg);
                cout << "Entrou GOTO" << endl;
                //take off
                Coordinate currentP(200.0,200.0,70.0);
                UAV u(id);
                Task gotoc(u, currentP);
                //gotoc.setType(Task::GOTO);

                gotoc.setType(Task::GOTO);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);

            }else if(!strcmp(msg.getMsg(), "pos2")){
                cout << "Entrou GOTO" << endl;
                //take off
                Coordinate currentP(400.0,400.0,70.0);
                UAV u(id);
                Task gotoc(u, currentP);
                //gotoc.setType(Task::GOTO);

                gotoc.setType(Task::GOTO);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);

            }else if(!strcmp(msg.getMsg(), "pos3")){
                cout << "Entrou GOTO" << endl;
                //take off
                Coordinate currentP(600.0,600.0,70.0);
                UAV u(id);
                Task gotoc(u, currentP);
                //gotoc.setType(Task::GOTO);

                gotoc.setType(Task::GOTO);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);

            }else if(!strcmp(msg.getMsg(), "decolar")){
                //take off
                for (int i = 0; i < ms->getSize(); i++) {
                    Coordinate currentP(50.0,50.0,100.0);
                    UAV u(i);
                    cout << "i: " << i << endl;
                    Task gotoc(u, currentP);
                    gotoc.setType(10);
                    TaskManager t;
                    t.addTask(gotoc);

                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    taskMessage.setDestination(msg.getDestination());
                    taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                    comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
                }
            }else if(!strcmp(msg.getMsg(), "quarteirao")){
                //take off
                Coordinate currentP(500.0,500.0,400.0);
                UAV u(id);
                Task gotoc(u, currentP);
                gotoc.setType(Task::FLY_AROUND_SQUARE);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
            }else if(!strcmp(msg.getMsg(), "quarteiraoT")){
                //take off
                for (int i = 0; i < ms->getSize(); i++) {
                    Coordinate currentP(500.0,500.0,400.0);
                    UAV u(i);
                    Task gotoc(u, currentP);
                    gotoc.setType(Task::FLY_AROUND_SQUARE);
                    TaskManager t;
                    t.addTask(gotoc);

                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    taskMessage.setDestination(msg.getDestination());
                    taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
                    comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);

                }
            }else if(!strcmp(msg.getMsg(), "carro")){
                Coordinate currentP(300.0,420.0,90.0);
                UAV u(id);
                Task gotoc(u, currentP);
                gotoc.setType(Task::FLY_AROUND);
                gotoc.setUAV(u);
                TaskManager t;
                t.addTask(gotoc);

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                taskMessage.setDestination(msg.getDestination());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
            }
            comm.sendMessageToUAV(id, msg);
        }
        cout << "Join" << endl;
        conectar.join();
    }else{
        //when it fails enter here
        switch (serverSocket) {
            case -1:
                std::cerr << "Inaccessible ports" << std::endl;
                break;
            case -2:
                std::cerr << "Error establishing the server socket" << std::endl;
                break;
            case -3:
                std::cerr << "Error binding socket to local address" << std::endl;
                break;
        }
        exit(0);
    }
}

void assignPreprogrammedTasks(int n, MysStatusManager *ms, MysCommunication comm){
    //Instanciar o planner juntamente com as tarefas
    //Atribuir as tarefas pros drones que ele quer (por meio do planner)
    //começar a missão
    //Pegar cada tarefa predefinida e dar um assync pra cada uma

    TaskManager t;
    //Building find sheep task
    /*cout << "Assigned task: Find sheep!" << endl;
    Coordinate initialPoint(500.0,500.0,600.0);
    UAV u(0);
    Task findsheep(u, Task::FIND_SHEEP, initialPoint);


    t.addTask(findsheep);*/


    //MissionPlanner *mp;
    //mp->assignAllTasks();

    //Enviando para os UAVs

    char assuntoMSG[10] = "New Task!";

    for (int itUAV = 0; itUAV < n; itUAV++) {
        UAV u = ms->getUAV(itUAV);
        int codeMessage = Message::TASK_MESSAGE;
        for (int itTask = 0; itTask < t.getNumTasks(u); itTask++) {
            //Enviando tarefa
            TaskMessage taskMessage(assuntoMSG, Message::TASK_MESSAGE);
            //taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
            taskMessage.setTask(t.getTaskByIndex(u, itTask));
            comm.sendTaskMessageToUAV(u.getNetworkConfigurations().getIdSocket(), taskMessage);
            cout << "ID of assigned task: " << itTask << endl;
            cout << "ID of UAV: " << u.getID() << endl;
            cout << "Number of uav tasks: " << t.getNumTasks(u) << endl;
        }
    }
}

int main(int argc, char const *argv[]){
	//This number identification will be important to set database execution
    RepositoryBase rep;
    int numExecution = rep.createExecutionID();
    cout << "EXECUTION ID: " << numExecution << endl;

    listenCommunication();

    std::cout << "Fim da execução" << std::endl;
    return 0;
}