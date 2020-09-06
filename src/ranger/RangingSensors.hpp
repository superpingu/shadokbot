// a class initializing the Time-of-flight ranging sensors and providing access to them

#ifndef RANGINGSENSORS_HPP
#define RANGINGSENSORS_HPP

#include <Wire.h>
#include "ranger/vl53l1x.hpp"

class RangingSensors {
	VL53L1X* sensors[6];
	int active_sensor; // index of the sensor currently doing measurements, or -1 if none

public:
	RangingSensors(TwoWire* port);

	// get the distance (in mm) from the center of the robot in the given direction (in deg)
	// the direction is relative to the robot, 0 being in front, then rotating counter-clockwise
	// The sensor closer to the specified direction will be selected.
	// The sensor is automatically activated when a read is requested and stay active until a read
	// in another direction is requested. This method is non-blocking so it might be necessary to
	// poll until it stops returning -1.
	int getDistance(int direction);
};

#endif
