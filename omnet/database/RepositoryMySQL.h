#ifndef MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#define MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#include "../../src/database/Repository.h"

namespace mysterio {

class RepositoryMySQL : public Repository {
public:
    RepositoryMySQL();
    virtual ~RepositoryMySQL();
    virtual bool createConnection();
    virtual void saveStatusInformation(int params);
    virtual int requestStatusInformation();
    virtual void destroyConnection();
};

}

#endif
