#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "motion/AbsoluteMotion.hpp"

static const MotionElement diamondPath[] = {
	{.x = 0, .y = 0, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 100, .y = 100, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 0, .y = 200, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = -100, .y = 100, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 0, .y = 0, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};

static const MotionElement homologPath[] = {
	{.x = 145, .y = 930, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 145, .y = 730, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 645, .y = 730, .heading = 45, .speed = 200, .strategy = MOVE_TURN},
	{.x = 295, .y = 1080, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	{.x = 145, .y = 1080, .heading = 0, .speed = 200, .strategy = TURN_RECAL},
	{.x = 145, .y = 930, .heading = 0, .speed = 200, .strategy = MOVE_TURN},
	END_PATH
};

#endif
