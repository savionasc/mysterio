#ifndef MYSTERIO_SRC_AGGREGATOR_SIMPLEUAV_H_
#define MYSTERIO_SRC_AGGREGATOR_SIMPLEUAV_H_

class SimpleUAV { //Aqui eu pensei que seria um exemplo simples duma instância dum uav, tipo isso
    // MOVER PARA UTILS E MUDAR PARA UAV
public:
    SimpleUAV();
    virtual ~SimpleUAV();
    virtual int getID();
    virtual void setID(int id);
    virtual double getXAxis();
    virtual void setXAxis(double xAxis);
    virtual double getYAxis();
    virtual void setYAxis(double yAxis);
    virtual double getZAxis();
    virtual void setZAxis(double zAxis);
    virtual int getTempoDeVoo();
    virtual void setTempoDeVoo(int tempodevoo);
    virtual double getVelocidade();
    virtual void setVelocidade(double velocidade);


protected:
    int iD;
    int tempoDeVoo;
    double xAxis;
    double yAxis;
    double zAxis;
    double velocidade;
    //bateria
    //disponivel
};

#endif /* MYSTERIO_SRC_AGGREGATOR_SIMPLEUAV_H_ */
