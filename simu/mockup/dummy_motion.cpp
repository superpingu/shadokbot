#include "motion/Motion.hpp"
#include "stdlib.h"

bool turnOngoing = false;
bool moveOngoing = false;
void (*next_cb)();

int32_t mMaxAcceleration;
int32_t mMinSpeed;
int32_t mRecalSpeed;
int32_t mRecalDistance;
bool mEnable;

Motion::Motion() {
	// initialize all motors
	motor_F = new TMC5130(false, 0, 0);
	motor_RL = new TMC5130(false, 0, 0);
	motor_RR = new TMC5130(false, 0, 0);
}

void Motion::update()
{
	if (turnOngoing) {

	} else if (moveOngoing) {

	}
}

void Motion::recalSpeed(int32_t speed) {mRecalSpeed = speed;}
void Motion::recalDistance(int32_t distance) {mRecalDistance = distance;}
void Motion::turn(int32_t angle, int32_t angular_speed, void (*callback)())
{
	turnOngoing = true;
	//next_cb = callback;
}


void Motion::move(int32_t distance, int32_t angle, int32_t speed, void (*callback)(), bool recal)
{
	moveOngoing = true;
	//next_cb = callback;
}

void Motion::moveXY(int32_t deltaX, int32_t deltaY, int32_t speed, void (*callback)(), bool recal) {}
