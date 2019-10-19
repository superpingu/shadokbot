#ifndef MOTION_HPP
#define MOTION_HPP

#include "Motor.hpp"
#include "imu/IMU.hpp"

class Motion {
	void (*moveCallback)(); // called when a move is finished
	IMU* imu;
public:
	Motor *motor_FL, *motor_FR, *motor_RL, *motor_RR;

	Motion();

	// update motor speeds, must be called periodically
	void update();

	// set maximum allowed acceleration for the robot
	void maxAcceleration(int32_t acc);
	// set minimum speed of the robot
	void minSpeed(int32_t speed);
	// set recalibration speed
	void recalSpeed(int32_t speed);
	// recalibration: set slow down distance before hitting the wall/max overshoot distance
	void recalDistance(int32_t distance);
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
	//   recal: slow down at the end and stop when hitting a wall
	void move(int32_t distance, int32_t angle, int32_t speed, void (*callback)()=NULL, bool recal=false);

	// move the robot with a linear translation
	//   deltaX : translation along X axis (in mm, relative)
	//   deltaY : translation along Y axis (in mm, relative)
	//   speed: translation cruise speed (ramp-up and ramp-down excluded), in mm/s (must be > 0)
	//   callback: function to call when the move ends
	//   recal: slow down at the end and stop when hitting a wall
	void moveXY(int32_t deltaX, int32_t deltaY, int32_t speed, void (*callback)()=NULL, bool recal=false);
};

#endif
