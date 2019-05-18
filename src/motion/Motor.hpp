#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "../hal/Timer.hpp"

class Motor {
 	const uint8_t enablePin, clockPin, dirPin;
	const bool invDir;

	int32_t cruiseSpeed; // speed during the constant speed phase of the move (in half ticks / speed time unit)
	int32_t accelerationDistance; // distance the motor runs while accelerating before reaching cruise speed (in half ticks)
	int32_t accelerationCoeff; // used to compute speed from position during the move (fixed point position = MOTOR_ACC_COEFF_FP)

	bool recalibration; // true when current move ends with a recalibration
	bool recalPhase; // true when we are the slow constant speed phase before hitting the wall
public:
	Timer* motorTimer; // timer allowing to send step pulses periodically to the stepper motor controller
	int32_t position; // current position in the move (in half ticks)
	int32_t goalPosition; // position to reach to complete the move (in half ticks)
	uint32_t period;

	int32_t maxAcceleration; // maximum accelerating (in half ticks / speed time unit^2)
	int32_t minSpeed; // minumum allowed speed for the motor (in half ticks / speed time unit)
	int32_t recalSpeed; // speed during recalibration phase
	int32_t recalDistance; // distance traveled at recal speed/max overshoot distance

	Motor(uint8_t _enablePin, uint8_t _dirPin, uint8_t _clockPin, bool _invDir);

	void update(); // update motor speed, must be called periodically

	// move by a given distance (in half ticks) with a given cruise speed (in half ticks / speed time unit)
	void move(int32_t speed, uint32_t distance, bool recal=false);
	void enable(bool enabled); // enable stepper motor drive or not
	void stopRecal(); // stop the motor when recalibration is done

	bool finished(); // true when the motor is not moving
	bool recalibrating(); // true when the motor is in the recalibration phase

	void setPulse(bool active); // activate or not clock and dir outputs
};

#endif
