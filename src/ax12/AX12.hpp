/* AX12 : this class represents an AX12 digital actuator (or any compatible model)
 * always call AX12::init before creating instances (see AXcomms.hpp) */

#include <stdint.h>
#include <stdlib.h>
#include "AXcomms.hpp"

#ifndef AX12_HPP
#define AX12_HPP

#define MAX_AX12_INSTANCES 20 // maximum number of instances of AX12

enum AX12mode { // AX12 control mode
	DEFAULT_MODE, // control in position, the output can only rotate within a 300deg range
	WHEEL_MODE // free wheel mode, AX12 can do full turns but only speed can be controlled (not position)
};

class AX12 : public AXcomms {
	void (*endOfMoveCallback)(void); // callback called when a move finishes
	int noMotionCounter; // number of consecutive end of move polls where the AX12 was not moving
	AX12mode controlMode; // current control mode

	// used for end of move detection by polling moving AX12 :
	static int instanceNumber; // total number of instances of AX12
	static AX12* instances[MAX_AX12_INSTANCES]; // pointers to every AX12 instance

public:
	// must be called periodically to detect end of moves and call callbacks
	static void update();

	// control the AX12 with the provided axid. The function also sets a defaut config:
	//   AX12 response : all instruction
	//   response delay : 6us
	//   alarm shutdown : torque OFF on overheating, overload, voltage error
	//   alarm LED : LED blinks on overheating, overload, voltage error
	//   mode : default
	//   goal speed : 512 (50% of the max)
	//   max torque : 1023 (max value)
	AX12(uint8_t axid);

	int getPosition(); // returns current position between 0 and 1023, increasing clockwise
	int getSpeed(); // returns the current speed between -1023 and 1023 (positive is clockwise).
	int getLoad(); // returns the current load between -1023 and 1023 (positive is clockwise).
	int getStatus(); // get current AX12 status (error flags). See datasheet for more info.
	int getVoltage(); // get AX12 power voltage (in MILLIVOLTS)
	int getTemperature(); // get AX12 internal temperature in degrees celsius
	bool isMoving(); // returns true when AX12 is moving

	// set AX12 control mode:
	// return 0 for no error or error code (see AXcomms.hpp)
	int setMode(AX12mode mode);
	// set target speed between -1023 and 1023, positive speed is clockwise
	// return 0 for no error or error code (see AXcomms.hpp)
	int setSpeed(int speed);
	// set max torque from 0 to 1023. Setting torque to zero will disable drive
	// (preventing AX12 from moving), setting to any other value will enable drive.
	// return 0 for no error or error code (see AXcomms.hpp)
	int setTorque(int torque);
	// set rear LED state (true for ON)
	// return 0 for no error or error code (see AXcomms.hpp)
	int setLED(bool state);

	// move to the given position (between 0 and 1023, increasing clockwise) and
	// call (optional) callback when AX12 stops moving (when it reached the goal
	// position or if an error occured).
	// return 0 for no error or error code (see AXcomms.hpp)
	int move(int position, void (*callback)(void) = NULL);
	// Cancel an end move callback. Only the callback is cancelled, not the movement.
	void cancelCallback();

	// switch to wheel mode and turn at the specified speed (-1023 to 1023), positive is clockwise
	// return 0 for no error or error code (see AXcomms.hpp)
	int turn(double speed);
};

#endif
