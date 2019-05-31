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

void initRobot() {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(TEAM_SWITCH, INPUT_PULLUP);
    pinMode(MATCH_SWITCH, INPUT_PULLUP);
    pinMode(MODE_SWITCH, INPUT_PULLUP);
    pinMode(START_JACK, INPUT_PULLUP);

    armLeft = new AX12(ARM_LEFT_ID);
    armRight = new AX12(ARM_RIGHT_ID);
}

void deployArm(void (*callback)()) {
    armRight->setTorque(300);
    armLeft->setTorque(300);
    if(getTeam() == PURPLE)
        armLeft->move(512);
    else
        armRight->move(512);
}

void increaseArmTorque() {
    armRight->setTorque(500);
    armLeft->setTorque(500);
}

#define BATT_PROBE_COEFF (3270*(11+33)/11)
// returns battery voltage in millivolts
uint getBatteryVoltage() {
	uint value = analogRead(BATT_PROBE)*BATT_PROBE_COEFF;
	return (value >> 10);
}
