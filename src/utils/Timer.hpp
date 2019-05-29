// This class allows to configure an Arduino Due hardware timer to call a
// function periodically. (C++ class methods should not be used as callback).

#include <stdint.h>
#include <stdlib.h>

#ifndef TIMER_HPP
#define TIMER_HPP

#define US_TO_TIMER_PERIOD 42 // coeff to convert microseconds to timer period

struct Timer {
	void (*callback)(void*); // callback called when timer expires
	void* callbackArg; // parameter passed as callback argument when timer expires
	uint32_t period;
	int timerID; // hardware timer/counter to use (from 0 to 8)

	Timer(int timerPeriod=42000);

	uint32_t getPeriod(); // returns period in timer ticks (42 ticks/us)
	void setPeriod(uint32_t timerPeriod); // set period in timer ticks (42 ticks/us)

	// set the function to call when the timer expires, and the argument to pass to the callback
	void setCallback(void (*cb)(void*), void* arg=NULL);

	void start(); // start timer (callback will be called after 1 period and periodically after that)
	void stop(); // stop timer
};

#endif
