#ifndef MOTION_HPP
#define MOTION_HPP

#include "Motor.hpp"

extern Motion* motion; // pointer to motion instance

class Motion {
private:

	int16_t max_acceleration;
	int16_t min_speed;

	void (*move_finished)(); // called when a move is finished
public:
	Motor *motor_FL, *motor_FR, *motor_RL, *motor_RR;

	Motion();

	// send ticks to the stepper motors at the appropriate frequency
	void update();

	// get or set maximum allowed acceleration for the robot
	int16_t maxAcceleration(int16_t acc = 0);

	// get or set minimum speed of the robot
	uint16_t minSpeed(int16_t speed = 0);

	// enable or disable the motors drive
	void enable(bool enabled);

	// call a function when an operation (a turn or a move) finishes
	void moveFinished(void (*func)());

	// turn without moving (no translation)
	//   angle: rotation in deg, relative, positive is counter-clockwise
	//   angular_speed: rotation speed in deg/s (should be positive)
	void turn(int16_t angle, uint16_t angular_speed);

	// move the robot with a linear translation
	//   distance: distance to move in mm (must be > 0)
	//   angle: direction in deg (0 is forward, 90 is leftward, -90 or 270 is rightward, ...)
	//   speed: translation cruise speed (ramp-up and ramp-down excluded), in mm/s (must be > 0)
	void move(uint16_t distance, int16_t angle, uint16_t speed);
};

#endif
