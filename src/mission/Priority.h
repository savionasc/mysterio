#ifndef MYSTERIO_SRC_MISSION_PRIORITY_H_
#define MYSTERIO_SRC_MISSION_PRIORITY_H_

class Priority {
public:
    Priority(){
        setPriority(0);
    }

    Priority(int priority){
        setPriority(priority);
    }

    virtual ~Priority(){}

    int getPriority() {
        return priority;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }

private:
    int priority;
};

#endif
