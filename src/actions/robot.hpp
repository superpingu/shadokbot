#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "motion/AbsoluteMotion.hpp"

enum RobotTeam { YELLOW = 0, PURPLE = 1, NOTEAM = 2 };

// sample buttons values
void sampleInputs();

// get selected team
RobotTeam getTeam();

// set the position of the robot to the first point of the path
void initPosition(const MotionElement* const path);


void initRobot();

unsigned int getBatteryVoltage();

#endif
