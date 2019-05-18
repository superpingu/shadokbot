#include "hal/Timer.hpp"
#include "ax12/AX12.hpp"
#include "motion/Motion.hpp"
#include "lidar/detection.hpp"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
Detection *detection;

void lilol() {
	delay(200);
	motion->enable(false);
}

void lol() {
	delay(100);
	motion->move(200, 90, 400, lilol);
}

// the setup function runs once when you press reset or power the board
void setup() {
	shell = new Shell(115200, getComms());
	detection = new Detection();

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);
	AX12::init(&Serial1, 115200);
	detection->init();
	pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
	delay(5);
	motion->update();
	shell->update();
	detection->update(motion->getPosX(), motion->getPosY(), motion->getMovementOrientation());
}
