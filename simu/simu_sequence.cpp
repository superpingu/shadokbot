#include "actions/sequence.hpp"
#include "simu_sequence.hpp"
#include "actions/robot.hpp"
#include <stdio.h>
#include <string.h>
#include "event.h"

static bool ongoingMove = false;
static bool initDone = false;

Sequence::Sequence(std::fstream* file)
{
	mFile = file;
	mStartingPos = {135, 1245, 270}; // default starting position
	if (mFile != NULL)
		read();
	if (path.empty()) {
		Event event = {EVENT_NEW_TARGET, mStartingPos};
		dispatchEvent(&event);
	} else {
		mStartingPos = path.front();
	}
}

void Sequence::read()
{
	if (mFile->is_open()) {
		mFile->seekg(0);
		std::string line;
		Position pos;
		int speed;
		do {
			getline(*mFile, line);
			if (sscanf(line.c_str(), "%d %d %d %d", &pos.x, &pos.y, &pos.angle, &speed) > 0) {
				printf("Going to %d %d (%d°) at %d\n", pos.x, pos.y, pos.angle, speed);
				path.push(pos);
			}
		} while (!mFile->eof());
		mFile->clear();
	}
}

void Sequence::setRobot(Robot* robot) {mRobot=robot;}

void endOfMoveCb() {
	ongoingMove = false;
}

void Sequence::onEvent(Event *event) {
	if (event->type == EVENT_NEW_TARGET) {
		path.push(event->targetEvent);
	} else if (event->type == EVENT_RESTART) {
		while (!path.empty())
			path.pop();
		initDone = false;
		ongoingMove = false;
		read();
	}
}

MotionElement* Sequence::buildPath(int targetX, int targetY, int angle) {
	static MotionElement path[10];
	printf("Build path from %d:%d to %d:%d", mRobot->getMotion()->getX(), mRobot->getMotion()->getY(), targetX, targetY);
	/* TODO avoid obstacle during move */
	path[0] = {.x = mRobot->getMotion()->getX(), .y = mRobot->getMotion()->getY(), .heading = mRobot->getMotion()->getHeading(), .speed = 200, .strategy = MOVE_TURN};
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
		mRobot->getMotion()->setX(mStartingPos.x);
		mRobot->getMotion()->setY(mStartingPos.y);
		mRobot->getMotion()->setHeading(mStartingPos.angle);
		mRobot->getMotion()->enable(true);
		initDone = true;
	} else if (!ongoingMove) {
		int speed;
		if (!path.empty()) {
			Position nextPos = path.front();
			path.pop();
			speed = 200;
			printf("Going to %d:%d (%d°)\n", nextPos.x, nextPos.y, nextPos.angle);
			mRobot->getMotion()->followPath(buildPath(nextPos.x, nextPos.y, nextPos.angle), endOfMoveCb);
			ongoingMove = true;
		}
	}
}

void Sequence::setStartingPos(Position startingPos)
{
	memcpy(&mStartingPos, &startingPos, sizeof(startingPos));
}
