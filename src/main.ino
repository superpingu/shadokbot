#include "hal/Timer.hpp"
#include "ax12/AX12.hpp"
#include "motion/Motion.hpp"

#include "board.h"
#include "lidar/lidar.hpp"
#include "ydlidar_arduino/YDLidar.h"

#include "shell/Shell.hpp"
#include "shell/commands.h"

Shell* shell;
Lidar* lidar;
YDLidar* ydLidar;
int count;

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

	ydLidar = new YDLidar();
	lidar = new Lidar();

	motion = new Motion();
	motion->enable(true);

	ydLidar->begin(Serial3, 128000);
	ydLidar->startScan();
	count = 0;
}

// the loop function runs over and over again forever
void loop() {
	delay(5);
//	motion->update();
	shell->update();
	while (Serial3.available() != 0) {
		lidar->pushSampleData(Serial3.read());
	}
	count++;
	if (count == 100) {
		uint32_t* map = lidar->getMap();
		for (int i = 0; i < 360; i++) {
			Serial.print(i);
			Serial.print(" ");
			Serial.println(map[i]);
		}
		count = 0;
	}
}
