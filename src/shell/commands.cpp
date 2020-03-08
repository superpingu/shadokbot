#include "Shell.hpp"
#include <Arduino.h>
#include "motion/AbsoluteMotion.hpp"
#include "actions/paths.hpp"

#define MAXPATHSIZE 50
static MotionElement shellPath[MAXPATHSIZE+1];
static int currentPathIndex = 0;
static bool pathLock = false;

static void moveCallback() {
	Serial.print("\nMove done.\n");
}

static void pathCallback() {
	Serial.print("\npath end\n");
	pathLock = false;
}
static void addPointCommand(int argc, char** argv) {
	if(argc != 6) {
		Serial.print("Oops ! Syntax: addpoint <x mm> <y mm> <heading deg> <speed mm/s> <strategy>\n");
		return;
	}
	if(currentPathIndex >= MAXPATHSIZE) {
		Serial.print("Max number of points reached already\n");
		return;
	}
	if(pathLock){
		Serial.print("Path execution running\n");
		return;
	}

	shellPath[currentPathIndex] = {
		.x = str2int(argv[1]),
		.y = str2int(argv[2]),
		.heading = str2int(argv[3]),
		.speed = str2int(argv[4]),
		.strategy = (MotionStrategy) str2int(argv[5])
	};
	currentPathIndex++;
}
static void execPathCommand(int argc, char** argv) {
	if(argc != 1) {
		Serial.print("Oops ! Syntax: execpath\n");
		return;
	}
	if(currentPathIndex == 0) {
		Serial.print("Add points to path first\n");
		return;
	}
	if(pathLock){
		Serial.print("Path execution running\n");
		return;
	}

	shellPath[currentPathIndex] = END_PATH;
	motion->followPath(shellPath, pathCallback);

	pathLock = true;
	currentPathIndex = 0;
}

static void moveCommand(int argc, char** argv) {
	if(argc != 4) {
		Serial.print("Oops ! Syntax: m <dist mm> <angle deg> <speed mm/s>\n");
		return;
	}
	motion->move(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]), moveCallback, true);
}

static void gotoCommand(int argc, char** argv) {
	if(argc != 6) {
		Serial.print("Oops ! Syntax: g <x mm> <y mm> <heading deg> <speed mm/s> <strategy>\n");
		return;
	}
	motion->goTo(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]), str2int(argv[4]),
		(MotionStrategy) str2int(argv[5]), moveCallback);
}

const command_t comms[] = {
	{"m", moveCommand},
	{"g", gotoCommand},
	{"addpoint", addPointCommand},
	{"execpath", execPathCommand},
	{NULL, NULL}
};

const command_t* getComms() { return comms; }
