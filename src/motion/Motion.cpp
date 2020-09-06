#include <Arduino.h>
#include "TMC5130.hpp"
#include "Motion.hpp"
#include "../board.h"
#include "motionconf.h"
#include "utils/trigo.hpp"

#define SIGN(a) (a > 0 ? 1 : -1)
#define ABS(a) ((a) > 0 ? (a) : -(a))

Motion::Motion() {
	moveCallback = NULL;
	minSpeed = DEFAULT_MIN_SPEED;
	maxAcceleration = DEFAULT_MAX_ACCELERATION;
	endTime = 0;

	// enable motor drivers output stage
	pinMode(TMC_ENABLEN, OUTPUT);
	digitalWrite(TMC_ENABLEN, LOW);

	// initialize all motor drivers
	motor_F = new TMC5130(F_INVDIR, F_CHIPSELECT, TMC_INTN);
	motor_RL = new TMC5130(RL_INVDIR, RL_CHIPSELECT, TMC_INTN);
	motor_RR = new TMC5130(RR_INVDIR, RR_CHIPSELECT, TMC_INTN);

	// use 4-points ramp
	motor_F->setTransitionSpeed(0);
	motor_RL->setTransitionSpeed(0);
	motor_RR->setTransitionSpeed(0);
	motor_F->setLowSpeedAcceleration(maxAcceleration, maxAcceleration);
	motor_RL->setLowSpeedAcceleration(maxAcceleration, maxAcceleration);
	motor_RR->setLowSpeedAcceleration(maxAcceleration, maxAcceleration);
}

void Motion::update() {
	if(!motor_F->isMoving() && !motor_RL->isMoving() && !motor_RR->isMoving() && endTime == 0 && moveCallback != NULL) {
		endTime = micros();
	}

	if(endTime != 0 && micros() > endTime + MOTION_BLANK_TIME) {
		endTime = 0;
		// save and set moveCallback to NULL before calling it (a new value might be given during call)
		void (*callback)() = moveCallback;
		moveCallback = NULL;
		callback();
	}
}

void Motion::recalSpeed(int32_t speed) {
}

void Motion::recalDistance(int32_t distance) {
}

void Motion::turn(int32_t angle, int32_t angular_speed, void (*callback)()) {
	int32_t dist = angle*DEG_TO_USTEP;
	uint32_t speed = angular_speed*DEG_TO_USTEP;
	moveCallback = callback;

	motor_F->setCruiseSpeed(speed);
	motor_F->setMinSpeed(minSpeed, minSpeed);
	motor_F->setHighSpeedAcceleration(maxAcceleration, maxAcceleration);
	motor_RL->setCruiseSpeed(speed);
	motor_RL->setMinSpeed(minSpeed, minSpeed);
	motor_RL->setHighSpeedAcceleration(maxAcceleration, maxAcceleration);
	motor_RR->setCruiseSpeed(speed);
	motor_RR->setMinSpeed(minSpeed, minSpeed);
	motor_RR->setHighSpeedAcceleration(maxAcceleration, maxAcceleration);

	motor_F->move(dist);
	motor_RL->move(dist);
	motor_RR->move(dist);
}

void Motion::move(int32_t distance, int32_t angle, int32_t speed, void (*callback)(), bool recal) {
	moveCallback = callback;

	float sin_a = msin(angle*M_PI/180.0);
	float cos_a_cos_30 = 0.866*mcos(angle*M_PI/180.0);
	float sin_a_sin_30 = sin_a/2;
	float coeff_f = -sin_a;
	float coeff_rl = sin_a_sin_30 - cos_a_cos_30;
	float coeff_rr = sin_a_sin_30 + cos_a_cos_30;

	int32_t dist = distance*MM_TO_USTEP;
	uint32_t spd = speed*MM_TO_USTEP;

	motor_F->setCruiseSpeed(ABS(spd*coeff_f));
	motor_F->setMinSpeed(ABS(minSpeed*coeff_f), ABS(minSpeed*coeff_f));
	motor_F->setHighSpeedAcceleration(ABS(maxAcceleration*coeff_f), ABS(maxAcceleration*coeff_f));
	motor_RL->setCruiseSpeed(ABS(spd*coeff_rl));
	motor_RL->setMinSpeed(ABS(minSpeed*coeff_rl), ABS(minSpeed*coeff_rl));
	motor_RL->setHighSpeedAcceleration(ABS(maxAcceleration*coeff_rl), ABS(maxAcceleration*coeff_rl));
	motor_RR->setCruiseSpeed(ABS(spd*coeff_rr));
	motor_RR->setMinSpeed(ABS(minSpeed*coeff_rr), ABS(minSpeed*coeff_rr));
	motor_RR->setHighSpeedAcceleration(ABS(maxAcceleration*coeff_rr), ABS(maxAcceleration*coeff_rr));

	motor_F->move(dist*coeff_f);
	motor_RL->move(dist*coeff_rl);
	motor_RR->move(dist*coeff_rr);
}

void Motion::moveXY(int32_t deltaX, int32_t deltaY, int32_t speed, void (*callback)(), bool recal) {
	moveCallback = callback; // set end of move callback

	// convert to ustep units
	int32_t dX = (deltaX == 0 ? 0 : deltaX)*MM_TO_USTEP;
	int32_t dY = (deltaY == 0 ? 0 : deltaY)*MM_TO_USTEP;
	int32_t spd = speed*MM_TO_USTEP;
	// compute distance to travel, sign doesn't matter, we'll take absolute value later
	float dist = dX == 0 || dY == 0 ? dX + dY : sqrt(((int64_t) dX)*((int64_t) dX) + ((int64_t) dY)*((int64_t) dY));
	// compute coefficient for each motor, see formulas in Motion::move
	float coeff_f = dX/dist;
	float coeff_rl = (dX/2 - 0.866*dY)/dist;
	float coeff_rr = (dX/2 + 0.866*dY)/dist;
	// use absolute values, only distances are signed and they are computed separately
	coeff_f = ABS(coeff_f);
	coeff_rl = ABS(coeff_rl);
	coeff_rr = ABS(coeff_rr);

	uint32_t minSpd = minSpeed*coeff_f;
	uint32_t maxAcc = maxAcceleration*coeff_f;
	motor_F->setCruiseSpeed(spd*coeff_f);
	motor_F->setMinSpeed(minSpd, minSpd*2);
	motor_F->setHighSpeedAcceleration(maxAcc, maxAcc*2);

	minSpd = minSpeed*coeff_rl;
	maxAcc = maxAcceleration*coeff_rl;
	motor_RL->setCruiseSpeed(spd*coeff_rl);
	motor_RL->setMinSpeed(minSpd, minSpd*2);
	motor_RL->setHighSpeedAcceleration(maxAcc, maxAcc*2);

	minSpd = minSpeed*coeff_rr;
	maxAcc = maxAcceleration*coeff_rr;
	motor_RR->setCruiseSpeed(spd*coeff_rr);
	motor_RR->setMinSpeed(minSpd, minSpd*2);
	motor_RR->setHighSpeedAcceleration(maxAcc, maxAcc*2);

	motor_F->move(-dX);
	motor_RR->move(dX/2 + 0.866*dY);
	motor_RL->move(dX/2 - 0.866*dY);
}

float Motion::getMotionProgress() {
	// use the motor with the largest distance to travel for more accurate result
	if(motor_F->getMoveGoal() > motor_RL->getMoveGoal() && motor_F->getMoveGoal() > motor_RR->getMoveGoal() && motor_F->getMoveGoal() != 0) {
		return ((float) motor_F->getMovePosition())/motor_F->getMoveGoal();
	} else if(motor_RL->getMoveGoal() > motor_RR->getMoveGoal() && motor_RL->getMoveGoal() != 0) {
		return ((float) motor_RL->getMovePosition())/motor_RL->getMoveGoal();
	} else if(motor_RR->getMoveGoal() != 0) {
		return ((float) motor_RR->getMovePosition())/motor_RR->getMoveGoal();
	}
	// no move has been executed yet
	return 1.0;
}
