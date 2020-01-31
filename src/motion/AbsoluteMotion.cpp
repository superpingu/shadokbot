#include <Arduino.h>
#include "AbsoluteMotion.hpp"
#include "motionconf.h"
#include "utils/trigo.hpp"
#include "utils/table.hpp"

AbsoluteMotion* motion; // pointer to absolute motion instance

void absmOnEndOfFirstTurn() {
	if(motion->currentMove.strategy == TURN_MOVE || motion->currentMove.strategy == TURN_RECAL)
		motion->currentHeading = motion->currentMove.heading;

	motion->currentMotionType = MOVE;

	int32_t deltaX = (motion->currentMove.x - motion->currentX)*mcos_deg(motion->currentHeading)
	               + (motion->currentMove.y - motion->currentY)*msin_deg(motion->currentHeading);
	int32_t deltaY = (motion->currentMove.y - motion->currentY)*mcos_deg(motion->currentHeading)
	               - (motion->currentMove.x - motion->currentX)*msin_deg(motion->currentHeading);

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

void absmOnPathPointReached() {
	motion->currentPathPoint++;
	if(motion->currentPath[motion->currentPathPoint].speed != 0) { // check next point is not the terminating element
		motion->goTo(motion->currentPath[motion->currentPathPoint], absmOnPathPointReached);
	} else { // path finished
		void (*cb)() = motion->followPathCallback;
		motion->followPathCallback = NULL;
		if(cb != NULL) cb();
	}
}

AbsoluteMotion::AbsoluteMotion() : Motion() {
	gotoCallback = NULL;
	currentX = 0;
	currentY = 0;
	currentHeading = 0;
	currentMotionType = NONE;
	currentMotionDirection = 0;
	turnSpeed = DEFAULT_TURN_SPEED;
}

void AbsoluteMotion::update() {
	static int i = 0;
	Motion::update();

	if(currentMotionType != NONE && (i++ % 25) == 0) {
		Serial.print("pos ");
		Serial.print(getX());
		Serial.print(" ");
		Serial.print(getY());
		Serial.print(" ");
		Serial.print(getHeading());
		Serial.print("\n");
	}
}

void AbsoluteMotion::followPath(const MotionElement* path, void (*callback)()) {
	setX(path[0].x);
	setY(path[0].y);
	setHeading(path[0].heading);
	currentPath = path;
	currentPathPoint = 0;
	followPathCallback = callback;

	absmOnPathPointReached();
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

	// compute absolute motion direction for Lidar
	float deltaX = me.x - currentX;
	float deltaY = me.y - currentY;
	if(deltaX == 0) {
		currentMotionDirection = deltaY > 0 ? 0 : 180;
	} else {
		currentMotionDirection = matan(deltaY/deltaX)*180.0/M_PI;
		if(deltaX < 0)
			currentMotionDirection += 180;
		currentMotionDirection = (currentMotionDirection + 270) % 360;
	}
}

int32_t AbsoluteMotion::getX() {
	if(currentMotionType == MOVE) {
		// use the motor with the largest goal position.
		// During a move, FL and RR have the same goal position, as well as FR and RL
		//TODO update
		Motor* largestGoalMotor = motor_RL->goalPosition > motor_RR->goalPosition ? motor_RL : motor_RR;
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
		//TODO update
		Motor* largestGoalMotor = motor_RL->goalPosition > motor_RR->goalPosition ? motor_RL : motor_RR;
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
		int newHeading = currentHeading + motor_F->position*(currentMove.heading - currentHeading)/motor_F->goalPosition;
		newHeading = newHeading < 0 ? newHeading + 360 : newHeading;
		newHeading = newHeading > 360 ? newHeading - 360 : newHeading;
		return newHeading;
	} else {
		return currentHeading;
	}
}
int AbsoluteMotion::getMotionDirection() {
	return currentMotionDirection;
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

// stop the robot as fast as possible (with deceleration). It has no effect on rotations
void AbsoluteMotion::emergencyStop() {
	if(currentMotionType == MOVE) {
		motor_F->emergencyStop();
		motor_RL->emergencyStop();
		motor_RR->emergencyStop();
	}
}
// resume the move stopped by emergency stop
void AbsoluteMotion::emergencyResume() {
	motor_F->emergencyResume();
	motor_RL->emergencyResume();
	motor_RR->emergencyResume();
}

bool AbsoluteMotion::isOnSlopes() {
	int x = getX();
	int y = getY();

	if ((y > 0) && (y < SLOPES_END_Y)
	&& (x > SLOPES_START_X) && (x < SLOPES_END_X)) {
		return true; // Base of the slopes
	} else {
		return false;
	}
}
