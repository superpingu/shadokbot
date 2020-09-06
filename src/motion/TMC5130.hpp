#ifndef TMC5130_HPP
#define TMC5130_HPP

#include <stdint.h>
#include <stdlib.h>

// TMC5130 clock frequency (in Hz)
#define TMC5130_CLKFREQ 12000000

class TMC5130 {
  int cs_pin, int_pin;
  int xtarget, xmovestart;
  bool moving; // true when the motor is turning, updated when isMoving() is called

  void write_data(uint8_t address, uint32_t data);
  uint32_t read_data(uint8_t address);
public:
  TMC5130(bool _inv_direction, int _cs_pin, int _int_pin);

  // set accelerations, absolute in ustep/s2
  void setLowSpeedAcceleration(int acceleration, int deceleration); // acceleration when speed is between start speed and transition speed
  void setHighSpeedAcceleration(int acceleration, int deceleration); // acceleration when speed is higher than transition speed

  // set speeds, absolute in ustep/s
  void setMinSpeed(int startspeed, int stopspeed); // jump to startspeed when starting a ramp, jump from stopspeed to 0 when ending a ramp
  void setTransitionSpeed(int speed); // transition speed between the 2 accelerations
  void setCruiseSpeed(int speed); // maximum speed
  // return current speed in microstep/s, signed
  int getSpeed();

  void setTargetPosition(int position); // set target position in ustep
  int getPosition(); // get actual position in ustep

  void move(int distance); // move of a given distance relative to last target position, in ustep, relative
  int getMovePosition(); // get distance traveled since last target position, in ustep, relative
  int getMoveGoal(); // get goal distance of last (or current if any) move

  bool isMoving(); // true when a move is on-going

  void emergencyStop(); // start slowing down immediately to stop as fast as possible (within acceleration limits)
  void emergencyResume(); // resume a move stopped by emergency stop
};

#endif
