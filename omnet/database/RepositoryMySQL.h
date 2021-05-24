#ifndef MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#define MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#include <mysql/mysql.h>
#include "../../src/database/Repository.h"
#include "../../src/utils/Coordinate.h"

class RepositoryMySQL : public Repository {
public:
    RepositoryMySQL();
    virtual ~RepositoryMySQL();
    virtual bool createConnection();
    virtual void saveStatusInformation(int params);
    virtual int requestStatusInformation();
    virtual void destroyConnection();

    Coordinate getUAVLocation(int idUAV);
    void setUAVLocation(int idUAV, Coordinate coord);

protected:
    MYSQL connection;
};

#endif
