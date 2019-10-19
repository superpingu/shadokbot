#include "motion/Motor.hpp"

Motor::Motor(uint8_t _enablePin, uint8_t _dirPin, uint8_t _clockPin, bool _invDir) :
    enablePin(_enablePin),
    dirPin(_dirPin),
    clockPin(_clockPin),
    invDir(_invDir),
	motorTimer(NULL),
	position(1),
	goalPosition(1),
	period(1),
	maxAcceleration(1),
	minSpeed(0),
	recalSpeed(1),
	recalDistance(1)
{

}

void Motor::update()
{

}

void Motor::move(int32_t speed, uint32_t distance, bool recal)
{

}

void Motor::enable(bool enabled)
{

}

void Motor::stopRecal()
{

}

void Motor::emergencyStop()
{

}

void Motor::emergencyResume()
{

}

bool Motor::finished()
{
	return false;
}

bool Motor::recalibrating()
{
	return false;
}

void Motor::setPulse(bool active)
{

}
