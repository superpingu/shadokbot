#ifndef ABSOLUTEMOTION_HPP
#define ABSOLUTEMOTION_HPP

#include "Motion.hpp"

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

enum MotionType { NONE, MOVE, TURN };

// functions used as internal callbacks, DO NOT CALL
void absmOnEndOfFirstTurn();
void absmOnEndOfMove();
void absmOnEndOfGoTo();

class AbsoluteMotion : public Motion {
	// current position, or position at the beginning of the move when moving
	int32_t currentX;
	int32_t currentY;
	int currentHeading;

	MotionType currentMotionType; // current robot activity : no motion, turning or moving
	MotionElement currentMove; // current (or last) move being executed
	void (*gotoCallback)();

	friend void absmOnEndOfFirstTurn();
	friend void absmOnEndOfMove();
	friend void absmOnEndOfGoTo();
public:
	int32_t turnSpeed; // rotation speed in deg/s (should be positive)

	AbsoluteMotion();

	// update motor speeds, must be called periodically
	void update();

	// go to a position in absolute coordinates
	void goTo(int32_t x, int32_t y, int heading, int32_t speed, MotionStrategy strategy, void (*callback)()=NULL);
	void goTo(MotionElement me, void (*callback)()=NULL);

	int32_t getX(); // current absolute x in mm
	int32_t getY(); // current absolute y in mm
	int getHeading(); // current absolute heading in deg, 0-360 counter-clockwise

	// DO NOT MODIFY POSITION DURING A MOVE
	void setX(int32_t x); // set x position in mm
	void setY(int32_t y); // set y position in mm
	void setHeading(int heading); // set heading in deg
};

extern AbsoluteMotion* motion; // pointer to absolute motion instance

#endif
