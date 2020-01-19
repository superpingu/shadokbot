#include "actions/sequence.hpp"
#include "simu_sequence.hpp"
#include "actions/robot.hpp"
#include <stdio.h>
#include "actions/paths.hpp"

static bool ongoingMove = false;
static bool initDone = false;
static bool pendingMove = false;
static int nextX, nextY;
static sf::RenderWindow *table;

void initSequence(sf::RenderWindow *window) {
	table = window;
}

void endOfMoveCb() {
	ongoingMove = false;
}

void onEventSequence(sf::Event *event) {
	if (event->type == sf::Event::MouseButtonPressed && event->mouseButton.button == sf::Mouse::Left) {
		pendingMove = true;
		nextX = event->mouseButton.x * 2;
		nextY = event->mouseButton.y * 2;
		printf("Event %d %d\n", nextX, nextY);
	}
}

MotionElement* buildPath(int targetX, int targetY) {
	static MotionElement path[10];
	printf("Build path from %d:%d to %d:%d", motion->getX(), motion->getY(), targetX, targetY);
	/* TODO avoid obstacle during move */
	path[0] = {.x = motion->getX(), .y = motion->getY(), .heading = motion->getHeading(), .speed = 200, .strategy = MOVE_TURN};
	path[1] = {.x = targetX, .y = targetY, .heading = motion->getHeading(), .speed = 200, .strategy = MOVE_TURN};
	path[2] = END_PATH;
	return path;
}

const MotionElement* const* wrapPath(MotionElement* path) {
	static MotionElement* pathArray[2];
	pathArray[0] = path;
	pathArray[1] = path;
	return pathArray;
}

void sequenceUpdate() {
	if (!initDone) {
		initPosition(paletsPath);
		motion->enable(true);
		sampleInputs();
		initDone = true;
	} else if (!ongoingMove && pendingMove) {
		printf("Going to %d:%d\n", nextX, nextY);
		pendingMove = false;
		followPath(wrapPath(buildPath(nextX, nextY)), endOfMoveCb);
		ongoingMove = true;
	}
}
