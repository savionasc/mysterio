#ifndef MYSTERIO_SRC_UTILS_NETWORKCONFIGURATIONS_H_
#define MYSTERIO_SRC_UTILS_NETWORKCONFIGURATIONS_H_

class NetworkConfigurations {
public:
    NetworkConfigurations(){}
    virtual ~NetworkConfigurations(){}

    int getIdSocket() const {
        return idSocket;
    }

    void setIdSocket(int idSocket) {
        this->idSocket = idSocket;
    }

    int idSocket;
};

#endif
