#include "robot.hpp"
#include "board.h"
#include "ax12/AX12.hpp"

static RobotTeam team = NOTEAM;

static AX12* armLeft;
static AX12* armRight;

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

// set the position of the robot to the first point of the path
void initPosition(const MotionElement* const path) {
    motion->setX(path[0].x);
    motion->setY(path[0].y);
    motion->setHeading(path[0].heading);
}

void initRobot() {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(TEAM_SWITCH, INPUT_PULLUP);
    pinMode(MATCH_SWITCH, INPUT_PULLUP);
    pinMode(MODE_SWITCH, INPUT_PULLUP);
    pinMode(START_JACK, INPUT_PULLUP);
}


#define BATT_PROBE_COEFF (3270*(11+33)/11)
// returns battery voltage in millivolts
unsigned int getBatteryVoltage() {
	unsigned int value = analogRead(BATT_PROBE)*BATT_PROBE_COEFF;
	return (value >> 10);
}
