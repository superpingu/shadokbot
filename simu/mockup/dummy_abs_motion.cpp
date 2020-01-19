#include "motion/AbsoluteMotion.hpp"
#include "simu_time.hpp"
#include <string.h>
#include <stdio.h>
AbsoluteMotion* motion;
#define SIGN(x) ((x) < 0 ? -1 : 1)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define DEG_TO_RAD(x) (((x) * M_PI) / 180)
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)
#define ANGULAR_SPEED 1
typedef enum Direction {
	FORWARD,
	BACKWARD
} Direction_t;

static bool onGoingMove = false;

AbsoluteMotion::AbsoluteMotion() {
	memset(&currentMove, 0, sizeof(MotionElement));
}

static int computeNewHeading(int currentHeading, int targetHeading, bool allowReverse)
{
	int deltaHeading = targetHeading - currentHeading;
	while(deltaHeading > 180) deltaHeading -= 360;
	while(deltaHeading <= -180) deltaHeading += 360;
	if (allowReverse && ABS(deltaHeading) > 90) {
		targetHeading += 180;
		while (targetHeading > 180) targetHeading -= 360;
		return computeNewHeading(currentHeading, targetHeading, false);
	} else {
	currentHeading += SIGN(deltaHeading) * MIN(ANGULAR_SPEED,ABS(currentHeading-targetHeading)%360) * (Time::getCurTime() - Time::getPrevTime());
		while (currentHeading > 180)
			currentHeading -= 360;
		while (currentHeading <= -180)
			currentHeading += 360;
		return currentHeading;
	}
}

void AbsoluteMotion::update()
{
	int newX, newY;
	Direction_t direction;
	if (currentMove.speed == 0)
		return;

	if (currentX != currentMove.x || currentY != currentMove.y) {
		// Compute motion direction
		currentMotionDirection = RAD_TO_DEG(atan2(currentMove.y-currentY,currentMove.x - currentX));
		printf("x %d/%d y %d/%d heading %d/%d speed %d\n", currentX, currentMove.x, currentY, currentMove.y, currentHeading, currentMotionDirection, currentMove.speed);

		if ((currentHeading != currentMotionDirection) && (currentHeading != currentMotionDirection + 180) && (currentHeading != currentMotionDirection - 180)) {
			setHeading(computeNewHeading(currentHeading, currentMotionDirection, true));
		} else {
			direction = currentHeading == currentMotionDirection ? FORWARD : BACKWARD;
			uint32_t deltaTime = Time::getCurTime() - Time::getPrevTime();
			if (direction == FORWARD) {
				newX = currentX + (deltaTime * currentMove.speed * cos(DEG_TO_RAD(currentHeading))) / 100;
				newY = currentY + (deltaTime * currentMove.speed * sin(DEG_TO_RAD(currentHeading))) / 100;
			} else {
				newX = currentX + (deltaTime * currentMove.speed * cos(DEG_TO_RAD(currentHeading + 180))) / 100;
				newY = currentY + (deltaTime * currentMove.speed * sin(DEG_TO_RAD(currentHeading + 180))) / 100;
			}
			if (SIGN(newX - currentMove.x) != SIGN(currentX - currentMove.x) || SIGN(newY - currentMove.y) != SIGN(currentY - currentMove.y)) {
				newX = currentMove.x;
				newY = currentMove.y;
			}
			currentX = newX;
			currentY = newY;
		}
	} else if (currentHeading != currentMove.heading) {
		setHeading(computeNewHeading(currentHeading, currentMove.heading, false));
		printf("heading %d/%d\n", currentHeading, currentMove.heading);
	} else { // Current move finished
		if (gotoCallback) {
			void (*cb)() = gotoCallback;
			gotoCallback = NULL;
			cb();
			return;
		}
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

void AbsoluteMotion::followPath(const MotionElement* path, void (*callback)())
{
	printf("followPath\n");
	setX(path[0].x);
	setY(path[0].y);
	setHeading(path[0].heading);
	currentPath = path;
	currentPathPoint = 0;
	followPathCallback = callback;

	absmOnPathPointReached();
}
void AbsoluteMotion::goTo(int32_t x, int32_t y, int heading, int32_t speed, MotionStrategy strategy, void (*callback)())
{
	MotionElement me;
	me.x = x;
	me.y = y;
	me.heading = heading;
	me.speed = speed;
	me.strategy = strategy;
	goTo(me, callback);
}

void AbsoluteMotion::goTo(MotionElement me, void (*callback)())
{
	memcpy(&currentMove, &me, sizeof(MotionElement));

	gotoCallback = callback;
	printf("goto\n");
}
int32_t AbsoluteMotion::getX() {return currentX;}
int32_t AbsoluteMotion::getY() {return currentY;}
int AbsoluteMotion::getHeading() {return currentHeading;}
int AbsoluteMotion::getMotionDirection() {return currentMotionDirection;}
void AbsoluteMotion::setX(int32_t x) {currentX = x;}
void AbsoluteMotion::setY(int32_t y) {currentY = y;}
void AbsoluteMotion::setHeading(int heading) {currentHeading = heading;}
void AbsoluteMotion::emergencyStop() {}
void AbsoluteMotion::emergencyResume() {}
bool AbsoluteMotion::isOnSlopes() {return false;}
