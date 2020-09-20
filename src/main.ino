#include "ax12/AX12.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "lidar/detection.hpp"
#include "actions/sequence.hpp"
#include "actions/robot.hpp"
#include "display/SevenSegDisplay.h"
#include "board.h"
#include "ranger/RangingSensors.hpp"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
RangingSensors* rangers;
Detection *detection;

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

// the setup function runs once when you press reset or power the board
void setup() {
	shell = new Shell(115200, getComms(), onShellInvite);
	rangers = new RangingSensors(&RANGER_I2C);
	AX12::init(&AX12_SERIALPORT, 115200);

	motion = new AbsoluteMotion();

	detection = new Detection();
	detection->init();

	display.begin();

	initRobot();
}

#define LOOP_PERIOD_US 5000 // duration of each loop iteration
// the loop function runs over and over again forever
void loop() {
	unsigned long loopStart = micros();

	int dist = rangers->getDistance(120);
	if(dist != -1) {
		Serial.print(dist);
		Serial.print('\n');
	}
	motion->update();
	shell->update();
	AX12::update();

	while (Serial2.available())
		detection->lidar.pushSampleData(Serial2.read());
	detection->update();

	sequenceUpdate();

	unsigned long loopTime = micros() - loopStart;
	delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);
}
