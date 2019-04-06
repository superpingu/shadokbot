#include "board.h"
#include "shell/Shell.hpp"
#include "shell/commands.h"
#include "ax12/AX12.hpp"
#include "motion/Motion.hpp"
#include "motion/motionconf.h"
#include "hal/Timer.hpp"

Shell* sh;

// the setup function runs once when you press reset or power the board
void setup() {
	sh = new Shell(115200, getComms());

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);
	AX12::init(&Serial1, 115200);

	motion = new Motion();
	motion->enable(true);
	motion->move(100, 180, 600);
}

// the loop function runs over and over again forever
void loop() {
	delay(5);
	motion->update();
}
