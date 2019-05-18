#include <Arduino.h>
#include "AbsoluteMotion.hpp"
#include "motionconf.h"

AbsoluteMotion* motion; // pointer to absolute motion instance

void absmOnEndOfFirstTurn() {
	if(motion->currentMove.strategy == TURN_MOVE || motion->currentMove.strategy == TURN_RECAL)
		motion->currentHeading = motion->currentMove.heading;

	motion->currentMotionType = MOVE;

	// TODO optimize
	int32_t deltaX = (motion->currentMove.x - motion->currentX)*cos(motion->currentHeading*2*M_PI/360)
	               - (motion->currentMove.y - motion->currentY)*sin(motion->currentHeading*2*M_PI/360);
	int32_t deltaY = (motion->currentMove.x - motion->currentX)*sin(motion->currentHeading*2*M_PI/360)
	               + (motion->currentMove.y - motion->currentY)*cos(motion->currentHeading*2*M_PI/360);
	motion->moveXY(deltaX, deltaY, motion->currentMove.speed, absmOnEndOfMove, motion->currentMove.strategy == TURN_RECAL);
}
void absmOnEndOfMove() {
	motion->currentX = motion->currentMove.x;
	motion->currentY = motion->currentMove.y;

	if(motion->currentMove.strategy == MOVE_TURN) {
		motion->currentMotionType = TURN;

		int deltaHeading = motion->currentMove.heading - motion->currentHeading;
		while(deltaHeading > 180) deltaHeading -= 360;
		while(deltaHeading <= -180) deltaHeading += 360;

		motion->turn(deltaHeading, motion->turnSpeed, absmOnEndOfGoTo);
	} else {
		absmOnEndOfGoTo();
	}
}
void absmOnEndOfGoTo() {
	motion->currentMotionType = NONE;
	motion->currentHeading = motion->currentMove.heading;

	if(motion->gotoCallback != NULL) {
		void (*cb)() = motion->gotoCallback;
		motion->gotoCallback = NULL;
		cb();
	}
}

AbsoluteMotion::AbsoluteMotion() : Motion() {
	gotoCallback = NULL;
	currentX = 0;
	currentY = 0;
	currentHeading = 0;
	currentMotionType = NONE;
	turnSpeed = DEFAULT_TURN_SPEED;
}

void AbsoluteMotion::update() {
	Motion::update();

	if(currentMotionType != NONE) {
		Serial.print("x = ");
		Serial.print(getX());
		Serial.print(", y = ");
		Serial.print(getY());
		Serial.print(", heading = ");
		Serial.print(getHeading());
		Serial.print("\n");
	}
}

void AbsoluteMotion::goTo(int32_t x, int32_t y, int heading, int32_t speed, MotionStrategy strategy, void (*callback)()) {
	MotionElement me = {.x = x, .y = y, .heading = heading, .speed = speed, .strategy = strategy};
	goTo(me, callback);
}
void AbsoluteMotion::goTo(MotionElement me, void (*callback)()) {
	currentMove = me;
	gotoCallback = callback;

	if((me.strategy == TURN_MOVE || me.strategy == TURN_RECAL) && me.heading != currentHeading) {
		currentMotionType = TURN;

		int deltaHeading = me.heading - currentHeading;
		while(deltaHeading > 180) deltaHeading -= 360;
		while(deltaHeading <= -180) deltaHeading += 360;

		turn(deltaHeading, turnSpeed, absmOnEndOfFirstTurn);
	} else if(me.x != currentX || me.y != currentY) {
		absmOnEndOfFirstTurn();
	} else {
		absmOnEndOfMove();
	}
}

int32_t AbsoluteMotion::getX() {
	if(currentMotionType == MOVE) {
		// use the motor with the largest goal position.
		// During a move, FL and RR have the same goal position, as well as FR and RL
		Motor* largestGoalMotor = motor_FL->goalPosition > motor_FR->goalPosition ? motor_FL : motor_FR;
		int32_t goalPosition = largestGoalMotor->goalPosition;

		// apply correction to goalPosition when recalibrating to compensate Motor internal changes
		if(currentMove.strategy == TURN_RECAL) {
			goalPosition += (largestGoalMotor->recalibrating() ? -1 : 1)*largestGoalMotor->recalDistance;
		}

		return currentX + largestGoalMotor->position*(currentMove.x - currentX)/goalPosition;
	} else {
		return currentX;
	}
}
int32_t AbsoluteMotion::getY() {
	if(currentMotionType == MOVE) {
		// use the motor with the largest goal position.
		// During a move, FL and RR have the same goal position, as well as FR and RL
		Motor* largestGoalMotor = motor_FL->goalPosition > motor_FR->goalPosition ? motor_FL : motor_FR;
		int32_t goalPosition = largestGoalMotor->goalPosition;

		// apply correction to goalPosition when recalibrating to compensate Motor internal changes
		if(currentMove.strategy == TURN_RECAL) {
			goalPosition += (largestGoalMotor->recalibrating() ? -1 : 1)*largestGoalMotor->recalDistance;
		}

		return currentY + largestGoalMotor->position*(currentMove.y - currentY)/goalPosition;
	} else {
		return currentY;
	}
}
int AbsoluteMotion::getHeading() {
	if(currentMotionType == TURN) {
		int newHeading = currentHeading + motor_FL->position*(currentMove.heading - currentHeading)/motor_FL->goalPosition;
		newHeading = newHeading < 0 ? newHeading + 360 : newHeading;
		newHeading = newHeading > 360 ? newHeading - 360 : newHeading;
		return newHeading;
	} else {
		return currentHeading;
	}
}

void AbsoluteMotion::setX(int32_t x) {
	currentX = x;
}
void AbsoluteMotion::setY(int32_t y) {
	currentY = y;
}
void AbsoluteMotion::setHeading(int heading) {
	currentHeading = heading;
}
