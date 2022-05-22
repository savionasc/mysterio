#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "../../src/communication/Message.h"
#include "../../src/communication/StatusMessage.h"
#include "../../src/communication/TaskMessage.h"
#include "../../src/taskmanager/TaskManager.h"
#include "../status/MysStatusManager.h"
#include "MessageSender.cc"

using namespace std;

class MessageReceive{
public:
    void operator()(int param){
        while(waitMessage(param)){ }
    }

    bool waitMessage(int socket){
        message stmsg;
        memset(&stmsg, 0, sizeof(stmsg));
        recv(socket, (message*)&stmsg, sizeof(stmsg), 0);

        cout << "Recebido " << stmsg.typeMessage << endl;
        if(stmsg.typeMessage == Message::STRUCTSTATUS){
            cout << "Mensagem instancia 4: " << stmsg.typeMessage << endl;
            if(stmsg.typeMessage == 11){
                cout << "location: " << stmsg.location_x << endl;
            }else if(stmsg.typeMessage == 12){
                cout << "velocity: " << stmsg.velocity << endl;
            }else if(stmsg.typeMessage == 13){
                cout << "battery: " << stmsg.battery << endl;
            }
        }
        return true;
    }
};
