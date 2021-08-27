#ifndef MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_
#define MYSTERIO_SRC_MISSION_MISSIONPLANNER_H_

class MissionPlanner {
public:
    MissionPlanner(){}
    virtual ~MissionPlanner(){}

    //Task container
    std::vector<Task*> tasks[NUMUAVS];
};

#endif
