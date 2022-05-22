#include "ExampleCommunication.h"

#include <iostream>
#include "../communication/MysCommunication.h"
#include "../../src/database/RepositoryMySQL.h"
#include "../../src/mission/MissionPlanner.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../communication/UAVRegistry.cc"

#include "../../src/mission/Command.h"

using namespace std;

void assignPreprogrammedTasks(int, MysStatusManager*, MysCommunication);

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
            Message msg(m, 10, -1, id);

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

            if(!strcmp(msg.getMsg(), "decolar")){
                //take off
                for (int i = 0; i < ms->getSize(); i++) {
                    Coordinate currentP(50.0,50.0,100.0);
                    UAV u(i);
                    Task gotoc(u, currentP);
                    gotoc.setType(10);
                    TaskManager t;
                    t.addTask(gotoc);

                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
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

    cout << "Assigned task: Find sheep!" << endl;
    Coordinate initialPoint(500.0,500.0,600.0);
    UAV u(0);
    Task findsheep(u, Task::FIND_SHEEP, initialPoint);

    TaskManager t;
    t.addTask(findsheep);

    //MissionPlanner *mp;
    //mp->assignAllTasks();

    //Enviando para os UAVs

    char assuntoMSG[10] = "New Task!";

    for (int itUAV = 0; itUAV < n; itUAV++) {
        u = ms->getUAV(itUAV);
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
    RepositoryMySQL rep;
    int numExecution = rep.createExecutionID();
    cout << "EXECUTION ID: " << numExecution << endl;

    listenCommunication();

    std::cout << "Fim da execução" << std::endl;
    return 0;
}
