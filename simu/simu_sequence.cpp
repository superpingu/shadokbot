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
	mStartingPos = {135, 1245, 0}; // default starting position
}

void Sequence::init()
{
	if (mFile != NULL)
		read();
	if (path.empty()) {
		// Trigger event to set the initial position
		Event event = {EVENT_NEW_TARGET, {TARGET_EVENT_SRC_USER, mStartingPos}};
		dispatchEvent(&event);
	} else {
		SequenceElement element = path.front();
		mStartingPos = {element.action.motion->x, element.action.motion->y, element.action.motion->heading};
	}
}

void catch_buoy(void (*cb)()) {
	printf("catch_buoy\n");
}

void Sequence::read()
{
	if ((mFile != NULL) && mFile->is_open()) {
		mFile->seekg(0);
		std::string line;
		TargetEvent_t targetEvent;
		int speed;
		do {
			getline(*mFile, line);
			if (sscanf(line.c_str(), "%d %d %d %d", &targetEvent.target.x, &targetEvent.target.y, &targetEvent.target.angle, &speed) > 0) {
				printf("Going to %d %d (%d°) at %d\n", targetEvent.target.x, targetEvent.target.y, targetEvent.target.angle, speed);
				targetEvent.src = TARGET_EVENT_SRC_PATH_FILE;
				Event event = {EVENT_NEW_TARGET, targetEvent};
				dispatchEvent(&event); // Trigger an event to indicate a new point in path
			} else if (strcmp(line.c_str(), "catch_buoy") == 0) {
				Event event;
				event.type = EVENT_CATCH_BUOY;
				dispatchEvent(&event);
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
	printf("OnEvent\n");
	if (event->type == EVENT_NEW_TARGET) {
		SequenceElement element;
		MotionElement *motion = (MotionElement*)malloc(sizeof(MotionElement));
		motion->x = event->targetEvent.target.x;
		motion->y = event->targetEvent.target.y;
		motion->heading = event->targetEvent.target.angle;
		motion->speed = 200;
		motion->strategy = MOVE_TURN;
		element.type = SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT;
		element.action.motion = motion;
		path.push(element);
	} else if (event->type == EVENT_RESTART) {
		while (!path.empty()) {
			if (path.front().type == SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT)
				free(path.front().action.motion);
		}
			path.pop();
		initDone = false;
		ongoingMove = false;
		read();
	} else if (event->type == EVENT_CATCH_BUOY) {
		SequenceElement element = {.type = SEQUENCE_ELEMENT_TYPE_ACTION, {.cb=catch_buoy}};
		path.push(element);
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
		initDone = true;
	} else if (!ongoingMove) {
		if (!path.empty()) {
			SequenceElement nextPos = path.front();
			path.pop();
			if (nextPos.type == SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT) {
				printf("Going to %d:%d (%d°)\n", nextPos.action.motion->x, nextPos.action.motion->y, nextPos.action.motion->heading);
				mRobot->getMotion()->followPath(buildPath(nextPos.action.motion->x, nextPos.action.motion->y, nextPos.action.motion->heading), endOfMoveCb);
				free(nextPos.action.motion);
				ongoingMove = true;
			} else if (nextPos.type = SEQUENCE_ELEMENT_TYPE_ACTION) {
				nextPos.action.cb(endOfMoveCb);
			}
		}
	}
}

void Sequence::setStartingPos(Position startingPos)
{
	memcpy(&mStartingPos, &startingPos, sizeof(startingPos));
}
