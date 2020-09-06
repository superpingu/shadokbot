#ifndef ABSOLUTEMOTION_HPP
#define ABSOLUTEMOTION_HPP

#include "Motion.hpp"
#include "imu/IMU.hpp"

enum MotionStrategy {
	MOVE_TURN = 0, // translation then turn
	TURN_MOVE = 1, // turn then translation
	TURN_RECAL = 2 // turn then translation until robot hit a wall
};

struct MotionElement {
	int32_t x; // in mm
	int32_t y; // in mm
	int heading; // in deg, from 0 to 360, counter clockwise
	int32_t speed; // translation speed in mm/s
	MotionStrategy strategy;
};

// use this as last item to terminate a path sequence
#define END_PATH {0, 0, 0, 0, MOVE_TURN}

enum MotionType { NONE, MOVE, TURN };

// functions used as internal callbacks, DO NOT CALL
void absmOnEndOfFirstTurn();
void absmOnEndOfMove();
void absmOnEndOfGoTo();
void absmOnPathPointReached();

class AbsoluteMotion : public Motion {
	// current position, or position at the beginning of the move when moving
	int32_t currentX;
	int32_t currentY;
	int currentHeading;
	int currentMotionDirection;

	MotionType currentMotionType; // current robot activity : no motion, turning or moving
	MotionElement currentMove; // current (or last) move being executed
	const MotionElement* currentPath; // current (or last) path being followed
	int currentPathPoint;
	void (*gotoCallback)();
	void (*followPathCallback)();

	IMU* imu;

	friend void absmOnEndOfFirstTurn();
	friend void absmOnEndOfMove();
	friend void absmOnEndOfGoTo();
	friend void absmOnPathPointReached();
public:
	int32_t turnSpeed; // rotation speed in deg/s (should be positive)

	AbsoluteMotion();

	// update motor speeds, must be called periodically
	void update();

	// follow a path defined as a table of MotionElements and call callback when the robot has reached the end
	// The first element is used to set the initial position.
	// The last element must be followed by an element with speed at 0 to terminate the sequence.
	void followPath(const MotionElement* me, void (*callback)()=NULL);

	// go to a position in absolute coordinates
	void goTo(int32_t x, int32_t y, int heading, int32_t speed, MotionStrategy strategy, void (*callback)()=NULL);
	void goTo(MotionElement me, void (*callback)()=NULL);

	int32_t getX(); // current absolute x in mm
	int32_t getY(); // current absolute y in mm
	int getHeading(); // current absolute heading in deg, 0-360 counter-clockwise
	int getMotionDirection(); // current direction of the robot motion, absolute angle in deg, 0-360 counter-clockwise

	// DO NOT MODIFY POSITION DURING A MOVE
	void setX(int32_t x); // set x position in mm
	void setY(int32_t y); // set y position in mm
	void setHeading(int heading); // set heading in deg

	// stop the robot as fast as possible (with deceleration). It has no effect on rotations
	void emergencyStop();
	// resume the move stopped by emergency stop
	void emergencyResume();
};

extern AbsoluteMotion* motion; // pointer to absolute motion instance

#endif
