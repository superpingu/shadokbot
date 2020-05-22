#ifndef DISTANCE_SENSOR_HPP
#define DISTANCE_SENSOR_HPP

#include "eventHandler.hpp"

#define MAX_DISTANCE 3000

class DistanceSensor {
public:
	DistanceSensor(struct Position offset);
	void setPosition(struct Position position);
	int getDistance();

private:
	struct Position mPos;
	struct Position mOffset;
};

#endif // DISTANCE_SENSOR_HPP
