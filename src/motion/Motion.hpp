#ifndef MOTION_HPP
#define MOTION_HPP

#include "Motor.hpp"

class Motion {
private:
	void (*moveCallback)(); // called when a move is finished

public:
	Motor *motor_FL, *motor_FR, *motor_RL, *motor_RR;

	Motion();

	// update motor speeds, must be called periodically
	void update();

	// get or set maximum allowed acceleration for the robot
	void maxAcceleration(int16_t acc);
	// get or set minimum speed of the robot
	void minSpeed(int16_t speed);
	// enable or disable the motors drive
	void enable(bool enabled);

	// turn without moving (no translation)
	//   angle: rotation in deg, relative, positive is counter-clockwise
	//   angular_speed: rotation speed in deg/s (should be positive)
	//   callback: function to call when the turn ends
	void turn(int32_t angle, int32_t angular_speed, void (*callback)()=NULL);

	// move the robot with a linear translation
	//   distance: distance to move in mm (must be > 0)
	//   angle: direction in deg (0 is forward, 90 is leftward, -90 or 270 is rightward, ...)
	//   speed: translation cruise speed (ramp-up and ramp-down excluded), in mm/s (must be > 0)
	//   callback: function to call when the move ends
	void move(int32_t distance, int32_t angle, int32_t speed, void (*callback)()=NULL);

	int32_t getPosX();
	int32_t getPosY();
	int32_t getMovementOrientation();
};

extern Motion* motion; // pointer to motion instance

#endif
