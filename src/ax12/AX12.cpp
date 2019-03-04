#include <stdlib.h>
#include <Arduino.h>
#include "ax-constants.h"
#include "AXcomms.hpp"
#include "AX12.hpp"

int AX12::instanceNumber = 0; // total number of instances of AX12
AX12* AX12::instances[]; // pointers to every AX12 instance

// poll moving AX12s to call end of move callbacks at the end of the move
void AX12::update() {
	void (*callback)(void);

	// for each AX12 instance
	for(int i = 0; i < instanceNumber; i++) {
		// endOfMoveCallback is not NULL : AX12 is moving with callback to call at the end
		if(instances[i]->endOfMoveCallback != NULL) {
			// count the number of polls where the AX12 is not moving, there might be false positives
			// (sometimes the AX12 has not finished but still reports to be not moving)
			instances[i]->noMotionCounter = instances[i]->isMoving() ? 0 : instances[i]->noMotionCounter + 1;

			if(instances[i]->noMotionCounter >= 3) {
				callback = instances[i]->endOfMoveCallback; // save callback pointer
				instances[i]->noMotionCounter = 0;
				instances[i]->endOfMoveCallback = NULL;
				callback();
			}
		}
	}
}

AX12::AX12(uint8_t axid) : AXcomms(axid) {
	endOfMoveCallback = NULL;
	noMotionCounter = 0;
	
	// register this instance
	instances[instanceNumber++] = this;

	// setup AX12
	write8(AX_RETURN, 2); // AX12 respond to all instructions
	write16(AX_DELAY, 5); // return delay = 10us
	write8(AX_ALARM_SHUTDOWN, 0x25); // torque OFF on overheating, overload, voltage error
	write8(AX_ALARM_LED, 0x25); // LED blinks on overheating, overload, voltage error

	setMode(DEFAULT_MODE); // position control mode
	setTorque(1023); // enable drive and set torque at the max
	setSpeed(512); // set speed at 50% max
}

int AX12::getPosition() {
	uint16_t value;
	read16(AX_POS, &value);
	return value;
}
int AX12::getSpeed() {
	uint16_t value;
	read16(AX_SPEED, &value);
	return value & 0x0400 ? -(value & 0x3FF) : value & 0x3FF;
}
int AX12::getLoad() {
	uint16_t value;
	read16(AX_LOAD, &value);
	return value & 0x0400 ? -(value & 0x3FF) : value & 0x3FF;
}
int AX12::getStatus() {
	uint8_t status;
	ping(&status);
	return status;
}
int AX12::getVoltage() {
	uint8_t volt;
	read8(AX_VOLT, &volt);
	return ((int) volt)*100;
}
int AX12::getTemperature() {
	uint8_t temp;
	read8(AX_TEMP, &temp);
	return temp;
}
bool AX12::isMoving() {
	uint8_t value;
	verbose = false;
	read8(AX_MOVING, &value);
	verbose = true;
	return value == 1;
}

int AX12::setMode(AX12mode mode) {
	controlMode = mode;

	write16(AX_CW_LIMIT, 0); // make sure CW limit is correct (should always be 0)
	return write16(AX_CCW_LIMIT, mode == WHEEL_MODE ? 0 : 0x3FF);
}
int AX12::setSpeed(int speed) {
	uint16_t value = speed < 0 ? 0x400 + (-speed & 0x3FF) : speed & 0x3FF;
	return write16(AX_GOAL_SPEED, value);
}
int AX12::setTorque(int torque) {
	return write16(AX_GOAL_SPEED, torque & 0x3FF);
}
int AX12::setLED(bool state) {
	return write8(AX_LED, state ? 1 : 0);
}

int AX12::move(int position, void (*callback)(void)) {
	endOfMoveCallback = callback;

	// change mode if necessary
	if(controlMode != DEFAULT_MODE)
		setMode(DEFAULT_MODE);

	// make sure position is within valid limits and send command
	position = position < 0 ? 0 : position > 0x3FF ? 1023 : position;
	return write16(AX_GOAL_POS, position);
}
void AX12::cancelCallback() {
	endOfMoveCallback = NULL;
	noMotionCounter = 0;
}

int AX12::turn(double speed) {
	if(controlMode != WHEEL_MODE)
		setMode(WHEEL_MODE);
	return setSpeed(speed);
}
