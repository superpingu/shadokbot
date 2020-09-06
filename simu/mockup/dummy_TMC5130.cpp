#include "motion/TMC5130.hpp"

void TMC5130::write_data(uint8_t address, uint32_t data) {}
uint32_t read_data(uint8_t address) {return 0;}
TMC5130::TMC5130(bool _inv_direction, int _cs_pin, int _int_pin) {}
// set accelerations, absolute in ustep/s2
void TMC5130::setLowSpeedAcceleration(int acceleration, int deceleration) {} // acceleration when speed is between start speed and transition speed
void TMC5130::setHighSpeedAcceleration(int acceleration, int deceleration) {} // acceleration when speed is higher than transition speed
// set speeds, absolute in ustep/s
void TMC5130::setMinSpeed(int startspeed, int stopspeed) {} // jump to startspeed when starting a ramp, jump from stopspeed to 0 when ending a ramp
void TMC5130::setTransitionSpeed(int speed) {} // transition speed between the 2 accelerations
void TMC5130::setCruiseSpeed(int speed) {} // maximum speed
// return current speed in microstep/s, signed
int TMC5130::getSpeed() {return 0;}
void TMC5130::setTargetPosition(int position) {} // set target position in ustep
int TMC5130::getPosition() {return 0;} // get actual position in ustep
void TMC5130::move(int distance) {} // move of a given distance relative to last target position, in ustep, relative
int TMC5130::getMovePosition() {return 0;} // get distance traveled since last target position, in ustep, relative
int TMC5130::getMoveGoal() {return 0;} // get goal distance of last (or current if any) move
bool TMC5130::isMoving() {return false;} // true when a move is on-going
void TMC5130::emergencyStop() {} // start slowing down immediately to stop as fast as possible (within acceleration limits)
void TMC5130::emergencyResume() {} // resume a move stopped by emergency stop
