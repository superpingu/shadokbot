#include <Arduino.h>
#include "board.h"
#include "ranger/RangingSensors.hpp"

// number of the arduino pin connected to the sensors XSHUT pin
static const int XSHUT_PIN[] = {
	RANGER_0_XSHUT,
	RANGER_60_XSHUT,
	RANGER_120_XSHUT,
	RANGER_180_XSHUT,
	RANGER_240_XSHUT,
	RANGER_300_XSHUT
};
// I2C addresses of the sensors after setup
static const uint8_t I2C_ADDRESS[] = {
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26
};
// distance to add to the sensors measurements to get distances to the center of the robot (in mm)
static const int SENSORS_OFFSET[] = {
	0,
	0,
	0,
	0,
	0,
	0
};

RangingSensors::RangingSensors(TwoWire* port) {
	// turn off all sensors
	for(int i = 0; i < 6; i++) {
		pinMode(XSHUT_PIN[i], OUTPUT);
		digitalWrite(XSHUT_PIN[i], LOW);
	}
	delay(10);

	// port->setClock(400000); // use 400 kHz I2C
	port->begin();

	// enable the sensors 1 by 1
	for(int i = 0; i < 6; i++) {
		// enable sensor hardware and give it a few ms to start
		digitalWrite(XSHUT_PIN[i], HIGH);
		delay(10);
		// initialize and configure sensor
		sensors[i] = new VL53L1X();
		sensors[i]->setTimeout(500);

		if(!sensors[i]->init()) {
			Serial.print(i);
			Serial.println(" : Failed to detect and initialize sensor");
			break;
		}

		sensors[i]->setAddress(I2C_ADDRESS[i]);
		sensors[i]->setDistanceMode(VL53L1X::Short);
	  sensors[i]->setMeasurementTimingBudget(100000);
	}

	active_sensor = -1;
}

int RangingSensors::getDistance(int direction) {
	while(direction < 0) direction += 360;
	int selected_sensor = ((direction+30)/60) % 6;
	// start the sensor measurements if not active previously
	if(selected_sensor != active_sensor) {
		if(active_sensor > 0)
			sensors[active_sensor]->stopContinuous();
		sensors[selected_sensor]->startContinuous(100);
		active_sensor = selected_sensor;
	}
	// return the last measurement if any is available, don't wait
	//if(sensors[selected_sensor]->dataReady())
	return sensors[selected_sensor]->read() + SENSORS_OFFSET[selected_sensor];
	//else
		//return -1;
}
