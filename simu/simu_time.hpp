#ifndef SIMU_TIME_H
#define SIMU_TIME_H

#include "stdint.h"

class Time {
public:
	static uint32_t getCurTime();
	static uint32_t getPrevTime();
	static void increaseTime(uint32_t delta);
	static void reset();

private:
	static uint32_t prevTime;
	static uint32_t curTime;
};

#endif /* SIMU_TIME */
