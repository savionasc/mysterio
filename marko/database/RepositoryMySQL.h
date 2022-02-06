#ifndef MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#define MYSTERIO_OMNET_DATABASE_REPOSITORYMYSQL_H_
#include <mysql/mysql.h>
#include <string>
#include "../../src/database/Repository.h"
#include "../../src/utils/Coordinate.h"

class RepositoryMySQL : public Repository {
public:
    RepositoryMySQL();
    RepositoryMySQL(bool prints);
    virtual ~RepositoryMySQL();

    virtual bool createConnection();
    virtual void destroyConnection();

    int createExecutionID();
    int getExecutionID();

    Coordinate requestUAVLocation(int idUAV);
    void saveUAVLocation(int idUAV, Coordinate coord);
    double getVelocity(int idUAV);
    void setVelocity(int idUAV, double velocity);
    float getBattery(int idUAV);
    void setBattery(int idUAV, float battery);
    int getFlightTime(int idUAV);
    void setFlightTime(int idUAV, int flightTime);
    void enablePrints();
    void disablePrints();

protected:
    MYSQL connection;
    bool prints = false;

private:
    std::string getCurrentDate();
};

#endif
