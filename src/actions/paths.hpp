#ifndef PATHS_HPP
#define PATHS_HPP

#include "motion/AbsoluteMotion.hpp"

static const MotionElement testPathYellow[] = {
	{.x = 145, .y = 1235, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 145, .y = 730, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 474, .y = 732, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 474, .y = 1321, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 829, .y = 1321, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 829, .y = 1560, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 595, .y = 1560, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement testPathPurple[] = {
	{.x = 2855, .y = 1235, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2855, .y = 730, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2526, .y = 732, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2526, .y = 1321, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2171, .y = 1321, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2171, .y = 1560, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2405, .y = 1560, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement* const testPath[] = {testPathYellow, testPathPurple};

#endif