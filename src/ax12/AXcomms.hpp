// AXcomms provides the low level layer for communicating with the AX12

#include <stdint.h>
#include <stdlib.h>
#include <Arduino.h>

#ifndef AXCOMMS_HPP
#define AXCOMMS_HPP

// how many times we try again if an error is returned
#ifndef AX_SEND_RETRY
#define AX_SEND_RETRY 3
#endif

// how long we wait for an answer from AX12 (in ms)
#ifndef AX_MAX_ANSWER_WAIT
#define AX_MAX_ANSWER_WAIT 20
#endif

// return codes of communication functions
#define AX_NO_SERIAL -1 // serial port not initialized
#define AX_WRONG_CHECKSUM -2 // wrong checksum
#define AX_ID_MISMATCH -3 // target and answer ID mismatch
#define AX_TIMEOUT -4 // timeout (no answer received after AX_MAX_ANSWER_WAIT ms)

class AXcomms {
	static HardwareSerial* axserial;
	static bool initialized; // true when AX12 serial port has been setup

	int sendPacket(uint8_t instruction, uint8_t command, uint8_t arg1, uint8_t arg2, int argCount);
	int receiveAnswer(uint16_t* result, uint8_t* statusError);
	int transaction(uint8_t instruction, uint8_t command, uint16_t arg, int argCount, uint16_t* result, uint8_t* error);
public:
	bool verbose = true; // if true, error message are printed to the console when there is a communication error
	uint8_t id; // ID of the AX12 (between 0 and 254, 254 is broadcast)
	int errorCounter; // counter of the number of errors on read or reply

	/* initialize serial port. Calling this is MANDATORY before any other operation */
	static void init(HardwareSerial* serialport, int baudrate);

	AXcomms(uint8_t axid);

	// write some data in AX12 memory, 1 byte for axWrite8, 2 byte for axWrite16
	// arguments :
	//      address : the start address to write to
	//      value : the value to write in the memory
	//      statusError : the byte containing the error flags
	// returns 0 for OKAY, an error code otherwise (see above) */
	int write8(uint8_t address, uint8_t value, uint8_t* statusError = NULL);
	int write16(uint8_t address, uint16_t value, uint8_t* statusError = NULL);

	// read some data in AX12 memory, 1 byte for axRead8, 2 byte for axRead16
	// arguments :
	//      address : the start address to write to
	//      value : the value read
	//      statusError : the byte containing the error flags
	// returns 0 for OKAY, an error code otherwise (see above) */
	int read8(uint8_t address, uint8_t* value, uint8_t* statusError = NULL);
	int read16(uint8_t address, uint16_t* value, uint8_t* statusError = NULL);

	// ask for a status packet (returned in statusError),
	// returns 0 for OKAY, an error code otherwise (see above) */
	int ping(uint8_t* statusError = NULL);

	// reset AX12 memory to factory default. The communication may be broken as it
	// will set the baudrate to 1Mbps and ID to 1 */
	int factoryReset(uint8_t* statusError);
};

#endif
