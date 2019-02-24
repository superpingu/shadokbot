#include "Shell.hpp"
#include <Arduino.h>
//#include "motion/Motion.hpp"

/*static void moveCallback() {
	Serial.print("\nMove done.\n > ");
	motion->moveFinished(NULL);
}

static void moveCommand(int argc, char** argv) {
	if(argc != 4) {
		Serial.print("Oops ! Syntax: m <dist mm> <angle deg> <speed mm/s>\n");
		return;
	}
	motion->moveFinished(moveCallback);
	motion->move(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]));
}*/

void moveCommand(int argc, char** argv) {
	Serial.print("lool \n\r");
}

const command_t comms[] = {
	{"m", moveCommand},
	{NULL, NULL}
};

const command_t* getComms() { return comms; }
