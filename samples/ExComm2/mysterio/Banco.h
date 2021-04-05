#ifndef MYSTERIO_SAMPLES_EXCOMM2_MYSTERIO_BANCO_H_
#define MYSTERIO_SAMPLES_EXCOMM2_MYSTERIO_BANCO_H_
#include <mysterio/src/utils/Coordinate.h>

namespace inet {

class Banco {
public:
    Banco();
    virtual ~Banco();
    virtual void testeConexao();
    virtual void inserirDadosNoBanco();
    virtual void insertLocation(Coordinate coord, const char idUAV);
    virtual void consultarDadosDoBanco();
    virtual void alterarDadosDoBanco();
    virtual void deletarDadosDoBanco();
protected:
    #define HOST "localhost"
    #define USER "root"
    #define PASS "root"
    #define DB   "mestrado"
};

}

#endif
