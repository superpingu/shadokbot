#include "simu_time.hpp"

uint32_t Time::curTime = 0;
uint32_t Time::prevTime = 0;

uint32_t Time::getCurTime()
{
	return Time::curTime;
}

uint32_t Time::getPrevTime()
{
	return Time::prevTime;
}

void Time::increaseTime(uint32_t delta)
{
	Time::prevTime = Time::curTime;
	Time::curTime += delta;
}

void Time::reset()
{
	Time::curTime = 0;
	Time::prevTime = 0;
}
