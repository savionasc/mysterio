/*
 * Competence.h
 *
 *  Created on: Dec 14, 2021
 *      Author: savio
 */

#ifndef MYSTERIO_SRC_MISSION_COMPETENCE_H_
#define MYSTERIO_SRC_MISSION_COMPETENCE_H_
#include "Skill.h"

class Competence { //Or Responsability
public:
    Competence(){}
    Competence(int superiority){setSuperiorityDegree(superiority);}
    virtual ~Competence(){}

    int getSuperiorityDegree() {
        return superiorityDegree;
    }

    void setSuperiorityDegree(int superiorityDegree) {
        this->superiorityDegree = superiorityDegree;
    }

private:
    int superiorityDegree;
    Skill requirements[3];
};

#endif /* MYSTERIO_SRC_MISSION_COMPETENCE_H_ */
