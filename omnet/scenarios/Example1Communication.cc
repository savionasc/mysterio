#include "../scenarios/Example1Communication.h"
#include <iostream>
#include "../communication/CommunicationSocket.h"
#include "../../src/mission/MissionPlanner.h"

using namespace std;

#define PORT 1111
#define NUMUAVS 2
int conexoes[NUMUAVS];

void listenSocket(){ //Here starts the server communication
    CommunicationSocket comm;
    int serverSocket = comm.configureSocketServer(PORT);
    if(serverSocket > 0){
        thread conectar(ConnServerSocket(), serverSocket, conexoes);
        std::cout << "Waiting for a UAV to connect..." << std::endl;
        //waiting for the first UAV
        while(ct == -1){
        }

        //Atribuindo tarefas previamente (pós registro dos UAVs)

        cout << "Tarefa atribuída: Volta no carro" << endl;
        Coordinate currentP(300.0,420.0,90.0);
        UAV u(0);
        Task gotoc(u, currentP);
        gotoc.type = FLY_AROUND;
        gotoc.uav = u;
        TaskManager t;
        t.addTask(gotoc);

        //Enviando tarefa
        int codeMessage = TASK_MESSAGE;
        TaskMessage taskMessage("AAAA", TASK_MESSAGE);
        taskMessage.c = t.getTaskByIndex(u, t.getNumTasks(u)-1).target;
        taskMessage.task = t.getTaskByIndex(u, t.getNumTasks(u)-1);
        cout << "Criando tarefa com id: " << taskMessage.task.id << endl;
        CommunicationSocket comm;
        comm.sendTaskMessageToUAV(conexoes[0], taskMessage);

        //Antes desse loop é onde atribui tarefas previamente

        while(true){
            std::cout << "[C] Digite um UAV para receber:" << std::endl;
            int id;
            std::cin >> id;
            TaskManager t;
            UAV u(id);
            cout << "Tarefas desse UAV: " << t.getNumTasks(u) << endl;
            std::cout << "[C] Digite a mensagem:" << std::endl;
            char m[1500];
            std::cin >> m;
            Message msg(m, 10, -1, id);

            //Tratando mensagens ao digitar
            if(!strcmp(msg.getMsg(), "task")){
                TaskManager t;
                UAV u(0);
                std::cout << "Tarefas pro UAV0: " << t.getNumTasks(u) << "\n";
            }

            if(!strcmp(msg.getMsg(), "tasks")){
                TaskManager t;
                UAV u(0);
                std::cout << "Tarefas pro UAV0: " << t.getNumTasks(u) << "\n";
                vector<Task> ts = t.getTaskList(u);
                for (int i = 0; i < t.getNumTasks(u); i++) {
                    cout << "Task: " << ts[i].type << endl;
                    cout << "Status da tarefa: " << ts[i].status;
                    cout << "ID da tarefa: " << ts[i].id << endl;
                }
            }

            if(!strcmp(msg.getMsg(), "decolar")){ //take off
                for (int i = 0; i < NUMUAVS; i++) {
                    Coordinate currentP(50.0,50.0,100.0);
                    UAV u(i);
                    Task gotoc(u, currentP);
                    gotoc.type = 10;
                    TaskManager t;
                    t.addTask(gotoc);
                    cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                    //Enviando tarefa
                    int codeMessage = TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), TASK_MESSAGE);
                    taskMessage.c = t.getTaskByIndex(u, t.getNumTasks(u)-1).target;
                    taskMessage.task = t.getTaskByIndex(u, t.getNumTasks(u)-1);
                    cout << "Criando tarefa com id: " << taskMessage.task.id << endl;
                    comm.sendTaskMessageToUAV(conexoes[u.getID()], taskMessage);
                }
            }else if(!strcmp(msg.getMsg(), "quarteirao")){ //take off
                for (int i = 0; i < NUMUAVS; i++) {
                    Coordinate currentP(500.0,500.0,400.0);
                    UAV u(i);
                    Task gotoc(u, currentP);
                    gotoc.type = FLY_AROUND_SQUARE;
                    TaskManager t;
                    t.addTask(gotoc);
                    cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                    //Enviando tarefa
                    int codeMessage = TASK_MESSAGE;
                    TaskMessage taskMessage(msg.getMsg(), TASK_MESSAGE);
                    taskMessage.c = t.getTaskByIndex(u, t.getNumTasks(u)-1).target;
                    taskMessage.task = t.getTaskByIndex(u, t.getNumTasks(u)-1);
                    cout << "Criando tarefa com id: " << taskMessage.task.id << endl;
                    comm.sendTaskMessageToUAV(conexoes[u.getID()], taskMessage);
                }
            }else if(!strcmp(msg.getMsg(), "carro")){
                Coordinate currentP(300.0,420.0,90.0);
                UAV u(id);
                Task gotoc(u, currentP);
                gotoc.type = FLY_AROUND;
                gotoc.uav = u;
                TaskManager t;
                t.addTask(gotoc);
                cout << "UAV["<<u.getID()<<"]-Tasks: " << t.getNumTasks(u) << endl;

                //Enviando tarefa
                int codeMessage = TASK_MESSAGE;
                TaskMessage taskMessage(msg.getMsg(), TASK_MESSAGE);
                taskMessage.c = t.getTaskByIndex(u, t.getNumTasks(u)-1).target;
                taskMessage.task = t.getTaskByIndex(u, t.getNumTasks(u)-1);
                cout << "Criando tarefa com id: " << taskMessage.task.id << endl;
                comm.sendTaskMessageToUAV(conexoes[id], taskMessage);
            }
            comm.sendMessageToUAV(id, msg);
        }
        conectar.join();
    }else{ //when it fails enter here
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

    listenSocket();

    std::cout << "Fim da execução" << std::endl;
    return 0;
}
