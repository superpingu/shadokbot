#include "board.h"
#include "motion/Motion.hpp"
#include "shell/Shell.hpp"
#include "shell/commands.hpp"

Shell* sh;

// the setup function runs once when you press reset or power the board
void setup() {
	sh = new Shell(115200, comms);

	motion = new Motion();
	motion->enable(true);
}

// the loop function runs over and over again forever
void loop() {
	motion->update();
	sh->update();
}
