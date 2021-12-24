/*
 * Skill.h
 *
 *  Created on: Dec 14, 2021
 *      Author: savio
 */

#ifndef MYSTERIO_SRC_MISSION_SKILL_H_
#define MYSTERIO_SRC_MISSION_SKILL_H_

#include <iostream>
#include <cstring>

using namespace std;

class Skill {
public:
    Skill(){}
    //Skill(char *skill){
        //setSkill(skill);
    //}
    virtual ~Skill(){}

    int getValue() const {
        return value;
    }

    void setValue(int value) {
        this->value = value;
    }

    /*const char* getSkill() {
        return skill;
    }

    void setSkill(char *skill){
        strcpy(this->skill, skill);
    }*/

private:
    int value;
    //int a;
    //char skill[15];
};

#endif /* MYSTERIO_SRC_MISSION_SKILL_H_ */
