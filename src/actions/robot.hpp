#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "motion/AbsoluteMotion.hpp"

enum RobotTeam { YELLOW = 0, PURPLE = 1, NOTEAM = 2 };

// sample buttons values
void sampleInputs();

// get selected team
RobotTeam getTeam();

// follow the path of the selected team
void followPath(const MotionElement* const* path, void (*callback)()=NULL);

// set the position of the robot to the first point of the path (according to the team)
void initPosition(const MotionElement* const* path);

void deployArm(void (*callback)());
void increaseArmTorque();

void initRobot();

uint getBatteryVoltage();

#endif
