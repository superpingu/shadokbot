#include "Shell.hpp"
#include <Arduino.h>
#include "motion/AbsoluteMotion.hpp"

static void moveCallback() {
	//motion->enable(false);
	Serial.print("\nMove done.\n > ");
}

static void moveCommand(int argc, char** argv) {
	if(argc != 4) {
		Serial.print("Oops ! Syntax: m <dist mm> <angle deg> <speed mm/s>\n");
		return;
	}
	motion->enable(true);
	motion->move(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]), moveCallback, true);
}

static void gotoCommand(int argc, char** argv) {
	if(argc != 6) {
		Serial.print("Oops ! Syntax: m <x mm> <y mm> <heading deg> <speed mm/s> <strategy>\n");
		return;
	}
	motion->enable(true);
	motion->goTo(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]), str2int(argv[4]), str2int(argv[5]), moveCallback);
}

const command_t comms[] = {
	{"m", moveCommand},
	{"g", gotoCommand},
	{NULL, NULL}
};

const command_t* getComms() { return comms; }
