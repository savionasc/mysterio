#ifndef MYSTERIO_SRC_DATABASE_REPOSITORYBASE_H_
#define MYSTERIO_SRC_DATABASE_REPOSITORYBASE_H_
#include <mysql/mysql.h>
#include <string>
#include "../../src/status/UAVStatus.h"
#include "../../src/database/Repository.h"

class RepositoryBase : public Repository {
public:
    RepositoryBase();
    RepositoryBase(bool prints);
    virtual ~RepositoryBase();

    virtual bool createConnection();
    virtual void destroyConnection();

    int createExecutionID();
    int requestExecutionID();

    Coordinate requestUAVLocation(int idUAV);
    void saveUAVLocation(int idUAV, Coordinate coord);
    double getVelocity(int idUAV);
    void setVelocity(int idUAV, double velocity);
    float getBattery(int idUAV);
    void setBattery(int idUAV, float battery);
    int getFlightTime(int idUAV);
    void setFlightTime(int idUAV, int flightTime);
    bool isAvailable(int idUAV);
    void setAvailable(int idUAV, bool available);

    UAVStatus getUAVStatus(int idUAV);

    void enablePrints();
    void disablePrints();

    int getExecutionId();
    void setExecutionId(int executionId);

protected:
    MYSQL connection;
    bool prints = false;
    int executionID;

private:
    std::string getCurrentDate();
};

#endif
