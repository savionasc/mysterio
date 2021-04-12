#ifndef MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_
#define MYSTERIO_SRC_COMMUNICATION_COMMUNICABLE_H_

// deve ser implementado em todos os objetos comunicaveis (componentes, uav, etc)
//This interface is importante to create Communicables classes
class Communicable { //Classe Abstrata
public:
    //Communicable();
    //virtual ~Communicable();
    //definir o tipo message
    virtual void onMessageReceive();//message);
};

#endif
