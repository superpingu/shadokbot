#include "motion/AbsoluteMotion.hpp"
#include "simu_time.hpp"
#include <string.h>
#include <stdio.h>
#define EPSILON 0.2f
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
double curX, curY, curHeading, curMotionDirection;

static bool onGoingMove = false;

AbsoluteMotion::AbsoluteMotion() {
	memset(&currentMove, 0, sizeof(MotionElement));
}

static double computeNewHeading(double current, double target, bool allowReverse)
{
	double deltaHeading = target - current;
	while(deltaHeading > 180) deltaHeading -= 360;
	while(deltaHeading <= -180) deltaHeading += 360;
	printf("Delta %f\n", deltaHeading);
	if (allowReverse && ABS(deltaHeading) > 90) {
		target += 180;
		while (target > 180) target -= 360;
		return computeNewHeading(current, target, false);
	} else {
		curHeading += ((double)SIGN(deltaHeading)) * MIN(ANGULAR_SPEED * (Time::getCurTime() - Time::getPrevTime()), ABS(deltaHeading));
		while (curHeading > 180)
			curHeading -= 360;
		while (curHeading <= -180)
			curHeading += 360;
		return curHeading;
	}
}

void AbsoluteMotion::update()
{
	double newX, newY, speed, targetHeading;
	Direction_t direction;
	if (currentMove.speed == 0) // No move requested
		return;

	speed = (double)currentMove.speed;
	targetHeading = (double)currentMove.heading;
	while (targetHeading > 180)
		targetHeading -= 360.f;
	while (targetHeading <= -180)
		targetHeading += 360.f;
	if ((ABS(curX - currentMove.x) > EPSILON) || (ABS(curY - currentMove.y)  > EPSILON)) { // Target position not reached yet
		// Compute motion direction as straight line from current position to target position
		curMotionDirection = RAD_TO_DEG(atan2(currentMove.y-curY,currentMove.x - curX));
		printf("x %f/%d y %f/%d heading %f/%f speed %f\n", curX, currentMove.x, curY, currentMove.y, curHeading, curMotionDirection, speed);

		if ((ABS(curHeading - curMotionDirection) > EPSILON) && (ABS(curHeading - curMotionDirection + 180) > EPSILON) && (ABS(curHeading - curMotionDirection - 180) > EPSILON)) {
			curHeading = computeNewHeading(curHeading, curMotionDirection, true);
		} else {
			direction = ABS(curHeading - curMotionDirection) < EPSILON ? FORWARD : BACKWARD;
			uint32_t deltaTime = Time::getCurTime() - Time::getPrevTime();
			if (direction == FORWARD) {
				newX = curX + (deltaTime * speed * cos(DEG_TO_RAD(curHeading))) / 100;
				newY = curY + (deltaTime * speed * sin(DEG_TO_RAD(curHeading))) / 100;
			} else {
				newX = curX + (deltaTime * speed * cos(DEG_TO_RAD(curHeading + 180))) / 100;
				newY = curY + (deltaTime * speed * sin(DEG_TO_RAD(curHeading + 180))) / 100;
			}
			if (SIGN(newX - currentMove.x) != SIGN(curX - currentMove.x) || SIGN(newY - currentMove.y) != SIGN(curY - currentMove.y)) {
				newX = currentMove.x;
				newY = currentMove.y;
			}
			curX = newX;
			curY = newY;
		}
	} else if (ABS(curHeading - targetHeading) > EPSILON) {
		curHeading = computeNewHeading(curHeading, targetHeading, false);
		printf("heading %f/%f\n", curHeading, targetHeading);
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
	printf("Goto %d°\n", heading);
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
int32_t AbsoluteMotion::getX() {return (int32_t)curX;}
int32_t AbsoluteMotion::getY() {return (int32_t)curY;}
int AbsoluteMotion::getHeading() {return (int)curHeading;}
int AbsoluteMotion::getMotionDirection() {return (int)curMotionDirection;}
void AbsoluteMotion::setX(int32_t x) {curX = (double)x;}
void AbsoluteMotion::setY(int32_t y) {curY = (double)y;}
void AbsoluteMotion::setHeading(int heading) {curHeading = (double)heading;}
void AbsoluteMotion::emergencyStop() {}
void AbsoluteMotion::emergencyResume() {}
bool AbsoluteMotion::isOnSlopes() {return false;}
