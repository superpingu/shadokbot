#include "hal/Timer.hpp"
#include "ax12/AX12.hpp"
#include "motion/Motion.hpp"
#include "lidar/lidar.hpp"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
Lidar* lidar;

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
	lidar = new Lidar();

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);
	AX12::init(&Serial1, 115200);

	lidar->init(Serial3, 128000);
	lidar->startScan();

}

// the loop function runs over and over again forever
void loop() {
	delay(5);
	motion->update();
	shell->update();
	lidar->update();
}
