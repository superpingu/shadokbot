#include <stdlib.h>
#include <Arduino.h>
#include "ax-constants.h"
#include "AXcomms.hpp"

bool AXcomms::initialized = false; // true when AX12 serial port has been setup
HardwareSerial* AXcomms::axserial = NULL;

void AXcomms::init(HardwareSerial* serialport, int baudrate) {
	serialport->begin(baudrate);
	serialport->setTimeout(AX_MAX_ANSWER_WAIT);
	axserial = serialport;
	initialized = true;
}

// constructor checks the serial port is initialized at instantiation
AXcomms::AXcomms(uint8_t axid) {
	id = axid;
	errorCounter = 0;
	if(!initialized)
		Serial.print("ERROR : serial port not initialized\n");
}

int AXcomms::sendPacket(uint8_t instruction, uint8_t command, uint8_t arg1, uint8_t arg2, int argCount) {
	uint8_t checksum = ~(id + instruction + command + arg1 + arg2 + 2 + argCount);
	if(!initialized) {
		Serial.print("ERROR : serial port not initialized\n");
		return -1;
	}

	axserial->write(0xFF); axserial->write(0xFF);
	axserial->write(id);
	axserial->write(2+argCount);
	axserial->write(instruction);
	if(argCount>0)
		axserial->write(command);
	if(argCount>1)
		axserial->write(arg1);
	if(argCount==3)
		axserial->write(arg2);
	axserial->write(checksum);
	axserial->flush();
	axserial->read();

	return 0;
}

int AXcomms::receiveAnswer(uint16_t* result, uint8_t* statusError) {
	unsigned long startTime = micros();

	while(micros() - startTime < AX_MAX_ANSWER_WAIT*1000) {
		if(axserial->available() >= 6 && axserial->read() == 0xFF && axserial->read() == 0xFF) {
			uint8_t axid, length, error, checksum, arg1, arg2;
			int data;

			axid = axserial->read();
			length = axserial->read();
			error = axserial->read();

			if(length > 2) {
				arg1 = axserial->read();
			} else {
				arg1 = 0;
			}
			if(length > 3) {
				data = axserial->read();
				if(data == -1)
					return AX_TIMEOUT;
				arg2 = data;
			} else {
				arg2 = 0;
			}

			// get checksum
			data = axserial->read();
			if(data == -1)
				return AX_TIMEOUT;
			checksum = data;

			// make sure packet came back complete and without error
			if(((uint8_t)~(axid+length+error+arg1+arg2)) != checksum)
				return AX_WRONG_CHECKSUM;
			if(id != axid)
				return AX_ID_MISMATCH;
			if(statusError != NULL)
				*statusError = error;
			if(result != NULL)
				*result = arg1 + (arg2 << 8);
			return 0;
		}
		delayMicroseconds(200);
	}
	return AX_TIMEOUT;
}

int AXcomms::transaction(uint8_t instruction, uint8_t command, uint16_t arg, int argCount, uint16_t* result, uint8_t* error) {
	int code = 0;

	for(int i=0; i<AX_SEND_RETRY+1; i++) {
		while(axserial->available()) axserial->read(); // make sure there is no byte left in RX buffer

		code = sendPacket(instruction, command, arg&0xFF, (arg >> 8)&0xFF, argCount);
		if(id == 0xFE || code) break; // we got an error or we are broadcasting -> no answer

		code = receiveAnswer(result, error);
		if(!code) break; // if everything went well, return, otherwise retry
		errorCounter++; // otherwise increment error counter
	}

	if(verbose && code != 0) {
		Serial.print("AX12 communication ERROR with id=");
		Serial.print((int) id);
		switch(code) {
		case AX_NO_SERIAL:
			Serial.print(" : serial port not initialized\n");
			break;
		case AX_WRONG_CHECKSUM:
			Serial.print(" : wrong checksum\n");
			break;
		case AX_ID_MISMATCH:
			Serial.print(" : ID doesnt match\n");
			break;
		case AX_TIMEOUT:
			Serial.print(" : timeout\n");
			break;
		}
	}

	// wait some time for the AX12 to be ready for next command
	delayMicroseconds(400);

	return code;
}

int AXcomms::write8(uint8_t command, uint8_t arg, uint8_t* statusError) {
	return transaction(AX_WRITE_DATA, command, arg, 2, (uint16_t*)NULL, statusError);
}
int AXcomms::write16(uint8_t command, uint16_t arg, uint8_t* statusError) {
	return transaction(AX_WRITE_DATA, command, arg, 3, (uint16_t*)NULL, statusError);
}
int AXcomms::read8(uint8_t command, uint8_t* result, uint8_t* statusError) {
	int code;
	uint16_t result16;
	code = transaction(AX_READ_DATA, command, 1, 2, &result16, statusError);
	*result = (uint8_t) result16;
	return code;
}
int AXcomms::read16(uint8_t command, uint16_t* result, uint8_t* statusError) {
	return transaction(AX_READ_DATA, command, 2, 2, result, statusError);
}
int AXcomms::ping(uint8_t* statusError) {
	return transaction(AX_PING, 0, 0, 0, (uint16_t*)NULL, statusError);
}
int AXcomms::factoryReset(uint8_t* statusError) {
	return transaction(AX_RESET, 0, 0, 0, (uint16_t*)NULL, statusError);
}
