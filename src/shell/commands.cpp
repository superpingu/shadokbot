#include "Shell.hpp"
#include "RobotMotion.hpp"

static void moveCallback() {
	Serial.print("\nMove done.\n > ");
	shmotion->moveFinished(NULL);
}

static void moveCommand(int argc, char** argv) {
	if(argc != 4) {
		Serial.print("Oops ! Syntax: m <dist mm> <angle deg> <speed mm/s>\n");
		return;
	}
	shmotion->moveFinished(moveCallback);
	shmotion->move(str2int(argv[1]), str2int(argv[2]), str2int(argv[3]));
}

const command_t comms[] = {
	{"m", moveCommand},
	{NULL, NULL}
};
