#include "robot.hpp"
#include "board.h"
#include "ax12/AX12.hpp"

static RobotTeam team = NOTEAM;

// sample buttons values
void sampleInputs() {
    team = NOTEAM;
    team = getTeam();
}

// get selected team
RobotTeam getTeam() {
    if(team == NOTEAM)
        return digitalRead(TEAM_SWITCH) == HIGH ? YELLOW : PURPLE;
    else
        return team;
}

// follow the path of the selected team
void followPath(const MotionElement** path, void (*callback)()) {
    motion->followPath(path[getTeam() == YELLOW ? 0 : 1], callback);
}

// set the position of the robot to the first point of the path (according to the team)
void initPosition(const MotionElement** path) {
    const MotionElement* selectedPath = path[getTeam() == YELLOW ? 0 : 1];
    motion->setX(selectedPath[0].x);
    motion->setY(selectedPath[0].y);
    motion->setHeading(selectedPath[0].heading);
}
