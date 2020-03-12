#include "ax12/AX12.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "actions/robot.hpp"
#include "display/SevenSegDisplay.h"
#include "board.h"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
// Detection *detection;

// print shell invite, with battery voltage display
void onShellInvite() {
	int volts = getBatteryVoltage()/10;
	int integer_part = volts/100;
	int decimal_part =  volts - integer_part*100;
	Serial.print(integer_part);
	Serial.print('.');
	Serial.print(decimal_part);
	Serial.print("V > ");
}

void cb() {
	motion->goTo(0, 0, 0, 500, MOVE_TURN, cb);
}

const MotionElement path[] = {
	{.x = 0, .y = 0, .heading = 0, .speed = 300, .strategy = MOVE_TURN},
	{.x = -500, .y = 0, .heading = 90, .speed = 300, .strategy = TURN_MOVE},
	{.x = -500, .y = -500, .heading = 180, .speed = 300, .strategy = TURN_MOVE},
	{.x = 0, .y = -500, .heading = 270, .speed = 300, .strategy = TURN_MOVE},
	{.x = 0, .y = 0, .heading = 0, .speed = 300, .strategy = TURN_MOVE},
	END_PATH
};

// the setup function runs once when you press reset or power the board
void setup() {
	// AX12::init(&AX12_SERIALPORT, 115200);
	shell = new Shell(115200, getComms(), onShellInvite);
	motion = new AbsoluteMotion();

	// detection = new Detection();
	// detection->init();
	//
	// display.begin();

	// initRobot();
	delay(500);

	motion->followPath(path);
}

#define LOOP_PERIOD_US 40000 // duration of each loop iteration
// the loop function runs over and over again forever
void loop() {
	unsigned long loopStart = micros();

	motion->update();
	shell->update();
	// AX12::update();

	// while (Serial2.available())
	// 	detection->lidar.pushSampleData(Serial2.read());
	// detection->update();

	// sequenceUpdate();

	unsigned long loopTime = micros() - loopStart;
	delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);
}
