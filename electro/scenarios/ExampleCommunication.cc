#include "ExampleCommunication.h"

#include <iostream>
#include "../communication/MysCommunication.h"
#include "../../src/mission/MissionPlanner.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../communication/UAVRegistry.cc"

#include "../../src/mission/Command.h"

using namespace std;

void listenCommunication(){
    //Here starts the server communication

    int numeroDeUAVsEsperados = 4;
    MysStatusManager *ms;

    MysCommunication comm;
    //Port number
    int serverSocket = comm.configureSocketServer(1111);
    if(serverSocket > 0){
        thread conectar(UAVRegistry(), serverSocket);
        std::cout << "Waiting for a UAV to connect..." << std::endl;
        //waiting for the first UAV
        while(ms->getSize() < numeroDeUAVsEsperados){
        }

        //Atribuindo tarefas previamente (pós registro dos UAVs)

        //Instanciar o planner juntamente com as tarefas
        //Atribuir as tarefas pros drones que ele quer (por meio do planner)
        //começar a missão
        //Pegar cada tarefa predefinida e dar um assync pra cada uma

        cout << "Assigned task: Turn around the car" << endl;
        Coordinate currentP(300.0,420.0,90.0);
        UAV u(0);
        Coordinate gotoP(800.0,620.0,90.0);
        Task gotoa(u, Task::GOTO, gotoP);
        Task gotoc(u, Task::FLY_AROUND, currentP);

        TaskManager t;
        t.addTask(gotoa);
        t.addTask(gotoc);
        Coordinate currentR(500.0,500.0,400.0);
        u.setID(2);
        Task square(u, Task::FLY_AROUND_SQUARE, currentR);
        t.addTask(square);
        u.setID(3);
        square.setUAV(u);
        t.addTask(square);

        char conteudo[10] = "AAAAA";

        for (int itUAV = 0; itUAV < numeroDeUAVsEsperados; itUAV++) {
            u = ms->getUAV(itUAV);
            int codeMessage = Message::TASK_MESSAGE;
            for (int itTask = 0; itTask < t.getNumTasks(u); itTask++) {
                //Enviando tarefa
                TaskMessage taskMessage(conteudo, Message::TASK_MESSAGE);
                //taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));
                taskMessage.setTask(t.getTaskByIndex(u, itTask));
                comm.sendTaskMessageToUAV(u.getNetworkConfigurations().getIdSocket(), taskMessage);
                cout << "Number of UAV tasks: " << t.getNumTasks(u) << endl;
                cout << "Task sended: " << itTask << endl;
                cout << "UAV: " << u.getID() << endl;
            }
        }


        //Antes desse loop é onde atribui tarefas previamente

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
                    //cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    //taskMessage.setCoord(t.getTaskByIndex(u, t.getNumTasks(u)-1).getTarget());
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
                //cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                //Enviando tarefa
                int codeMessage = Message::TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                //taskMessage.setCoord(t.getTaskByIndex(u, t.getNumTasks(u)-1).getTarget());
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
                    //cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                    //Enviando tarefa
                    int codeMessage = Message::TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), Message::TASK_MESSAGE);
                    //taskMessage.setCoord(t.getTaskByIndex(u, t.getNumTasks(u)-1).getTarget());
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
                //taskMessage.setCoord(t.getTaskByIndex(u, t.getNumTasks(u)-1).getTarget());
                taskMessage.setTask(t.getTaskByIndex(u, t.getNumTasks(u)-1));

                comm.sendTaskMessageToUAV(ms->getUAV(u.getID()).getNetworkConfigurations().getIdSocket(), taskMessage);
            }
            comm.sendMessageToUAV(id, msg);
        }
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

int main(int argc, char const *argv[]){
	//This number identification will be important to set database execution
	int numExecution = 1; 

    listenCommunication();

    std::cout << "Fim da execução" << std::endl;
    return 0;
}
