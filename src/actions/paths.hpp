#ifndef PATHS_HPP
#define PATHS_HPP

#include "motion/AbsoluteMotion.hpp"

static const MotionElement paletsPathYellow[] = {
	{.x = 135, .y = 1245, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 135, .y = 650, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 550, .y = 650, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 550, .y = 1150, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 550, .y = 650, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1290, .y = 650, .heading = 225, .speed = 200, .strategy = MOVE_TURN},
	{.x = 500, .y = 1440, .heading = 225, .speed = 200, .strategy = MOVE_TURN},
	{.x = 600, .y = 1540, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 460, .y = 1540, .heading = 270, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1450, .y = 1540, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement paletsPathPurple[] = {
	{.x = 2865, .y = 1245, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2865, .y = 650, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2450, .y = 650, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2450, .y = 1150, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2450, .y = 650, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1710, .y = 650, .heading = 135, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2500, .y = 1440, .heading = 135, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2400, .y = 1540, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2540, .y = 1540, .heading = 90, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1550, .y = 1540, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement* const paletsPath[] = {paletsPathYellow, paletsPathPurple};

static const MotionElement acceleratorPathYellow[] = {
	{.x = 1450, .y = 1540, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1450, .y = 1830, .heading = 180, .speed = 200, .strategy = TURN_RECAL},
	{.x = 1570, .y = 1830, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1570, .y = 1750, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2230, .y = 1758, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2230, .y = 1800, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 2058, .y = 1800, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement acceleratorPathPurple[] = {
	{.x = 1550, .y = 1540, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1550, .y = 1830, .heading = 180, .speed = 200, .strategy = TURN_RECAL},
	{.x = 1430, .y = 1830, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 1430, .y = 1750, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 770, .y = 1758, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 770, .y = 1800, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	{.x = 942, .y = 1800, .heading = 180, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement* const acceleratorPath[] = {acceleratorPathYellow, acceleratorPathPurple};

static const MotionElement goldPathYellow[] = {
	{.x = 1660, .y = 1820, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 1660, .y = 1770, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 2275, .y = 1770, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 2275, .y = 1808, .heading = 180, .speed = 150, .strategy = TURN_RECAL},
	{.x = 2020, .y = 1808, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement goldPathPurple[] = {
	{.x = 1340, .y = 1820, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 1340, .y = 1770, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 725, .y = 1770, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	{.x = 725, .y = 1808, .heading = 180, .speed = 150, .strategy = TURN_RECAL},
	{.x = 980, .y = 1808, .heading = 180, .speed = 150, .strategy = MOVE_TURN},
	END_PATH
};
static const MotionElement* const goldPath[] = {goldPathYellow, goldPathPurple};

static const MotionElement chemin12PathYellow[] = {
	{.x = 200, .y = 0, .heading = 180, .speed = 300, .strategy = MOVE_TURN},
	{.x = 200, .y = 200, .heading = 180, .speed = 300, .strategy = TURN_RECAL},
	END_PATH
};
static const MotionElement chemin12PathPurple[] = {
	{.x = 2800, .y = 0, .heading = 180, .speed = 300, .strategy = MOVE_TURN},
	{.x = 2800, .y = 200, .heading = 180, .speed = 300, .strategy = TURN_RECAL},
	END_PATH
};
static const MotionElement* const chemin12Path[] = {chemin12PathYellow, chemin12PathPurple};

#endif