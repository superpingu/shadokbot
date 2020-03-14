#include "actions/sequence.hpp"
#include "simu_sequence.hpp"
#include "actions/robot.hpp"
#include <stdio.h>
#include "actions/paths.hpp"

static bool ongoingMove = false;
static bool initDone = false;
static bool pendingMove = false;
static int nextX, nextY, nextAngle;

Sequence::Sequence(const char *fileName)
{
	if (fileName != NULL)
		file.open(fileName);
}

void endOfMoveCb() {
	ongoingMove = false;
}

void Sequence::onEvent(sf::Event *event) {}

void Sequence::onNewTarget(int x, int y, int angle) {
	pendingMove = true;
	nextX = x;
	nextY = y;
	nextAngle = angle;
	printf("Event %d %d\n", nextX, nextY);
}

MotionElement* buildPath(int targetX, int targetY, int angle) {
	static MotionElement path[10];
	printf("Build path from %d:%d to %d:%d", motion->getX(), motion->getY(), targetX, targetY);
	/* TODO avoid obstacle during move */
	path[0] = {.x = motion->getX(), .y = motion->getY(), .heading = motion->getHeading(), .speed = 200, .strategy = MOVE_TURN};
	path[1] = {.x = targetX, .y = targetY, .heading = angle, .speed = 200, .strategy = MOVE_TURN};
	path[2] = END_PATH;
	return path;
}

const MotionElement* const* wrapPath(MotionElement* path) {
	static MotionElement* pathArray[2];
	pathArray[0] = path;
	pathArray[1] = path;
	return pathArray;
}

void Sequence::update() {
	if (!initDone) {
		motion->setX(paletsPath[0][0].x);
		motion->setY(paletsPath[0][0].y);
		motion->setHeading(paletsPath[0][0].heading);
		motion->enable(true);
		initDone = true;
	} else if (!ongoingMove) {
		int speed;
		if (file.is_open() && !pendingMove) {
			std::string line;
			getline(file, line);
			if (sscanf(line.c_str(), "%d %d %d %d", &nextX, &nextY, &nextAngle, &speed) > 0) {
				printf("Going to %d %d (%d°) at %d\n", nextX, nextY, nextAngle, speed);
				pendingMove = true;
			} else {
				file.close();
			}
		}
		if (pendingMove) {
			printf("Going to %d:%d (%d°)\n", nextX, nextY, nextAngle);
			pendingMove = false;
			motion->followPath(buildPath(nextX, nextY, nextAngle), endOfMoveCb);
			ongoingMove = true;
		}
	}
}
