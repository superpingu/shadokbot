#include "motion/AbsoluteMotion.hpp"
#include "simu_time.hpp"
#include <string.h>
#include <stdio.h>
AbsoluteMotion* motion;
#define SIGN(x) ((x) < 0 ? -1 : 1)
#define DEG_TO_RAD(x) (((x) * M_PI) / 180)
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)

static bool onGoingMove = false;

AbsoluteMotion::AbsoluteMotion() {
	memset(&currentMove, 0, sizeof(MotionElement));
}

void AbsoluteMotion::update()
{
	int newX, newY;
	if (currentMove.speed != 0) {
		printf("curX %d curY %d targetX %d targetY %d\n", currentX, currentY, currentMove.x, currentMove.y);
		if (currentX == currentMove.x && currentY == currentMove.y) {
			if (gotoCallback) {
				gotoCallback();
				return;
			}
		}
		// Compute heading
		if (currentMove.x != currentX) {
			currentHeading = RAD_TO_DEG(atan2(currentMove.y-currentY,currentMove.x - currentX));
		} else if (currentMove.y > currentY) {
			currentHeading = 90;
		} else {
			currentHeading = 270;
		}

		printf("x: %d y: %d speed %d heading %d\n", currentX, currentY, currentMove.speed, currentHeading);
		newX = currentX + (Time::getCurTime() - Time::getPrevTime()) * currentMove.speed/100 * cos(DEG_TO_RAD(currentHeading));
		newY = currentY + (Time::getCurTime() - Time::getPrevTime()) * currentMove.speed/100 * sin(DEG_TO_RAD(currentHeading));
		if (SIGN(newX - currentMove.x) != SIGN(currentX - currentMove.x) || SIGN(newY - currentMove.y) != SIGN(currentY - currentMove.y)) {
			newX = currentMove.x;
			newY = currentMove.y;
		}
		currentX = newX;
		currentY = newY;
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
