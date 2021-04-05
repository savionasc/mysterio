#ifndef MYSTERIO_SRC_AGGREGATOR_AGGREGATOR_H_
#define MYSTERIO_SRC_AGGREGATOR_AGGREGATOR_H_
#include <mysterio/src/aggregator/SimpleUAV.h>
#include <mysterio/src/utils/Coordinate.h>
#include <map>

class Aggregator {
public:
    Aggregator();
    virtual ~Aggregator();
    virtual void addUAV(SimpleUAV uav){
        this->uavs[uav.getID()] = uav;
    }
    virtual void removeUAV(int iD){
        uavs.erase(iD);
    }
    virtual int getNumUAVs(){
        return this->uavs.size();
    }
    virtual Coordinate getUAVLocation(int idUAV){
        SimpleUAV s = pegarUAV(idUAV);
        Coordinate c;
        c.x = s.getXAxis();
        c.y = s.getYAxis();
        c.z = s.getZAxis();
        return c;
    }
    virtual void setUAVLocation(Coordinate coord, int idUAV){
        //Não usar aqui Essa responsabilidade é para que tipo de classe? /ok
        SimpleUAV s = pegarUAV(idUAV);
        s.setXAxis(coord.x);
        s.setYAxis(coord.y);
        s.setZAxis(coord.z);
        this->uavs[idUAV] = s;
    }
    virtual double getUAVVelocity(int idUAV){
        SimpleUAV s = pegarUAV(idUAV);
        return s.getVelocidade();
    }
    virtual void setUAVVelocity(double velocity, int idUAV){
        SimpleUAV s = pegarUAV(idUAV);
        s.setVelocidade(velocity);
        this->uavs[idUAV] = s;
    }

protected: //Variáveis que fui criando e melhorando ainda
    std::map<int,SimpleUAV> uavs;
    int tempoDeFuncionamento; //do sistema
    SimpleUAV pegarUAV(int idUAV){
        SimpleUAV r;
        r.setID(-1);
        for (std::map<int, SimpleUAV>::iterator it=uavs.begin(); it!=uavs.end(); ++it)
            if(it->first == idUAV)
                return it->second;
        return r;
    }
    int numNodes;
};

#endif /* MYSTERIO_SRC_AGGREGATOR_AGGREGATOR_H_ */
