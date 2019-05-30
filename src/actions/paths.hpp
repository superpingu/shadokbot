#ifndef PATHS_HPP
#define PATHS_HPP

#include "motion/AbsoluteMotion.hpp"

static const MotionElement testPathYellow[] = {
	{.x = 135, .y = 1245, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 135, .y = 730, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 550, .y = 730, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 550, .y = 1150, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 550, .y = 605, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 1345, .y = 605, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 1245, .y = 705, .heading = 225, .speed = 250, .strategy = MOVE_TURN},
	{.x = 500, .y = 1430, .heading = 225, .speed = 250, .strategy = MOVE_TURN},
	{.x = 600, .y = 1530, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	{.x = 460, .y = 1530, .heading = 270, .speed = 250, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement testPathPurple[] = {
	{.x = 2865, .y = 1245, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2865, .y = 730, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2450, .y = 730, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2450, .y = 1150, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2450, .y = 605, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 1655, .y = 605, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 1755, .y = 705, .heading = 135, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2500, .y = 1430, .heading = 135, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2400, .y = 1530, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	{.x = 2540, .y = 1530, .heading = 90, .speed = 250, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement* const testPath[] = {testPathYellow, testPathPurple};

#endif