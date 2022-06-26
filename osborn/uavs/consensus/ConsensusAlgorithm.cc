#include "ConsensusAlgorithm.h"

ConsensusAlgorithm::ConsensusAlgorithm(Coordinate coord, std::vector<UAV> uavs){
    this->coordinate = coord;
    this->uavs = uavs;
}

ConsensusAlgorithm::~ConsensusAlgorithm() { }

//Este int será um enum.. ConsensusAlgorithm::SóPodeSubir
int ConsensusAlgorithm::run(){
    int radius = 50;
    int rad2 = 2 * radius;
    state = Collision::CASE_UP_OR_DOWN;
    inet::Coord uav = castCoordinateToCoord(coordinate);
    this->colisions.clear();

    int numColisions = 0;

    for(UAV u : uavs){
        UAVStatus us = u.getStatus();
        inet::Coord uavFriend = castCoordinateToCoord(us.getCoordinate());
        float distance = uav.distance(uavFriend);
        std::cout << "Distance between uav[" << u.getID() << "]:" << distance << std::endl;

        //Menor que 100 indica colisão dos campos potenciais artificiais
        if(distance < rad2){
            numColisions++;

            //verificar se o drone a colidir está em cima
            //se estiver em cima, mudar o status para não subir CASE_DOWN
            std::cout << "UAVS CAN COLLIDE!" << std::endl;


            //se tiver somente 1 colisão, o UAV que receber esta mensagem decide se desce ou sobe...
            //Se tiver pelo menos 2 colisões o state do uav atual nunca é CASE_UP_OR_DOWN
            if(numColisions > 1){
                if(state == Collision::CASE_UP_OR_DOWN){
                    if(uav.getZ() < uavFriend.getZ()){
                        state = Collision::CASE_DOWN;
                    }else{
                        state = Collision::CASE_UP;
                    }
                }else if(state == Collision::CASE_DOWN && uav.getZ() > uavFriend.getZ()){
                    state = Collision::CASE_NOT_UP_DOWN;
                }else if(state == Collision::CASE_UP && uav.getZ() < uavFriend.getZ()){
                    state = Collision::CASE_NOT_UP_DOWN;
                }
            }
            this->colisions.push_back(Collision(state, u));
        }
    }

    //verificando se tem mais de 1 colisão para corrigir o tipo de decisão que o UAV resolveu tomar
    if(numColisions > 1)
        for (int i = 0; i < colisions.size(); i++)
            colisions[i].setUAVCase(state);


    this->numColisions = numColisions;

    std::cout << "Retorno:" << state << std::endl;
    runned = true;
    return state;
}

void ConsensusAlgorithm::makeDecision(Collision collision){
    //Se tiver pelo menos 2 colisões o state do uav atual nunca é CASE_UP_OR_DOWN
    if(this->runned && this->numColisions > 1){
        std::cout << "Mais de uma colisão" << std::endl;
        if(state == Collision::CASE_UP
                && (collision.getUAVCase() == Collision::CASE_UP_OR_DOWN
                        || collision.getUAVCase() == Collision::CASE_DOWN)){
            std::cout << "Mesmo que não importe, vou subir e você desce" << std::endl;
            runned = 3;
        }else if(state == Collision::CASE_DOWN
                && (collision.getUAVCase() == Collision::CASE_UP_OR_DOWN
                        || collision.getUAVCase() == Collision::CASE_UP)){
            std::cout << "Mesmo que não importe, vou descer e você sobe" << std::endl;
            runned = 3;
        }else if(state == collision.getUAVCase() && state == Collision::CASE_NOT_UP_DOWN){
            std::cout << "Tomamos a mesma decisão, porém eu subo pouco e você desce pouco" << std::endl;
            runned = 2;
        }else{
            std::cout << "Tomamos a mesma decisão("<< state << "|" << collision.getUAVCase() <<"), porém a minha coordenada é mais perto e a sua mais longe" << std::endl;
            runned = 2;
        }
    }else if(this->runned && state == Collision::CASE_NOT_UP_DOWN){
        std::cout << "O ideal é que o outro faça alguma coisa... e eu não" << std::endl;
        std::cout << "Ou subir... talvez subir menos que o outro drone" << std::endl;
        runned = 2;
    }else{
        std::cout << "Uma colisão" << std::endl;
        switch (collision.UAVCase) {
            case Collision::CASE_UP_OR_DOWN:
                std::cout << "Mesmo que não importe, vou subir, então você desce" << std::endl;
                state = Collision::CASE_UP;
                runned = 3;
                break;
            case Collision::CASE_UP:
                std::cout << "Vou descer, então você sobe" << std::endl;
                state = Collision::CASE_DOWN;
                runned = 3;
                break;
            case Collision::CASE_DOWN:
                std::cout << "Vou subir, então você desce" << std::endl;
                state = Collision::CASE_UP;
                runned = 3;
                break;
            case Collision::CASE_NOT_UP_DOWN:
                std::cout << "Vou subir de qualquer maneira, então você desce" << std::endl;
                state = Collision::CASE_UP;
                runned = 2;
                break;
            default:
                std::cout << "Erro..." << std::endl;
                break;
        }
    }

}
Coordinate ConsensusAlgorithm::escapeCoordinate(){
    int distance = 40;
    std::cout << "state: " << state << std::endl;
    Coordinate aux = coordinate;
    if(state == Collision::CASE_DOWN && runned == 2)
        aux.setZ(aux.getZ()-(distance/2));
    else if(state == Collision::CASE_DOWN && runned == 3)
        aux.setZ(aux.getZ()-distance);
    else if(( state == Collision::CASE_UP || state == Collision::CASE_UP_OR_DOWN ) && runned == 2)
        aux.setZ(aux.getZ()+(distance/2));
    else if( (state == Collision::CASE_UP || state == Collision::CASE_UP_OR_DOWN) && runned == 3)
        aux.setZ(aux.getZ()+distance);

    return aux;
}

int ConsensusAlgorithm::getNumberOfCollisions(){
    return this->numColisions;
}

std::vector<Collision> ConsensusAlgorithm::getCollisions(){
    return this->colisions;
}

int ConsensusAlgorithm::getDecision(){
    return this->state;
}


//colision é uma classe que tem id do UAV que vai colidir e a proposta de desvio (pode ser cima e baixo ao mesmo tempo)
//Em vez de colision, pode usar um UAV e usar suas propriedades...

//Se tiver só um caso (ex: cima), aí o UAV atual fica com o oposto e retorna resposta accept
//Se tiver Ambos
//escapeColision(Colision colision){
//}
