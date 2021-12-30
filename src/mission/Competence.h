#ifndef MYSTERIO_SRC_MISSION_COMPETENCE_H_
#define MYSTERIO_SRC_MISSION_COMPETENCE_H_
#include "Skill.h"

class Competence {
public:
    Competence(){}
    virtual ~Competence(){}

    Skill getSkill() {
        return skill;
    }

    void setSkill(Skill skill) {
        this->skill = skill;
    }

    int getSuperiorityDegree() {
        return superiorityDegree;
    }

    void setSuperiorityDegree(int superiorityDegree) {
        this->superiorityDegree = superiorityDegree;
    }

private:
    int superiorityDegree;
    Skill skill;
};

#endif
