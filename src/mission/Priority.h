/*
 * Priority.h
 *
 *  Created on: Dec 13, 2021
 *      Author: savio
 */

#ifndef MYSTERIO_SRC_MISSION_PRIORITY_H_
#define MYSTERIO_SRC_MISSION_PRIORITY_H_

class Priority {
public:
    Priority(int priority){}

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

#endif /* MYSTERIO_SRC_MISSION_PRIORITY_H_ */
