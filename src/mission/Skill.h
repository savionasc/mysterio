/*
 * Skill.h
 *
 *  Created on: Dec 14, 2021
 *      Author: savio
 */

#ifndef MYSTERIO_SRC_MISSION_SKILL_H_
#define MYSTERIO_SRC_MISSION_SKILL_H_

#include <iostream>

class Skill {
public:
    Skill(){}
    Skill(char *skill){
        setSkill(skill);
    }
    virtual ~Skill(){}

    const char* getSkill() {
        return skill;
    }

    void setSkill(char *skill){
        std::strcpy(this->skill, skill);
    }

private:
    char skill[15];
};

#endif /* MYSTERIO_SRC_MISSION_SKILL_H_ */
