#include "hal/Timer.hpp"
#include "ax12/AX12.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "lidar/detection.hpp"

#include "board.h"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
Detection *detection;

int count = 0;

#define BATT_PROBE_COEFF (3270*(11+33)/11)
// returns battery voltage in millivolts
uint getBatteryVoltage() {
	uint value = analogRead(BATT_PROBE)*BATT_PROBE_COEFF;
	return (value >> 10);
}

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
	AX12::init(&AX12_SERIALPORT, 115200);

	detection = new Detection();
	detection->init();

	motion = new AbsoluteMotion();
	motion->enable(true);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LIDAR_M_SCTP, OUTPUT);
	analogWrite(LIDAR_M_SCTP, 250);
}

#define LOOP_PERIOD_US 5000 // duration of each loop iteration
// the loop function runs over and over again forever
void loop() {
	unsigned long loopStart = micros();
	motion->update();
	shell->update();
	detection->update(0, 0, 0);

	unsigned long loopTime = micros() - loopStart;
	delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);
}
