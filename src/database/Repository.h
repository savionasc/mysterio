#ifndef MYSTERIO_SRC_DATABASE_REPOSITORY_H_
#define MYSTERIO_SRC_DATABASE_REPOSITORY_H_

class Repository {
public:
    virtual bool createConnection() = 0;
    virtual void saveStatusInformation(int params) = 0;
    virtual int requestStatusInformation() = 0;
    virtual void destroyConnection() = 0;
};

#endif
