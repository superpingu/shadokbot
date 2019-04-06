#include "Arduino.h"
#include "Timer.hpp"

// pointers to timer instances
static Timer* timerInstances[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

struct timer_data_t {
	Tc* timer;
	uint32_t channel;
	IRQn_Type irq;
};
static const struct timer_data_t timerData[9] = {
	{TC0,0,TC0_IRQn},
	{TC0,1,TC1_IRQn},
	{TC0,2,TC2_IRQn},
	{TC1,0,TC3_IRQn},
	{TC1,1,TC4_IRQn},
	{TC1,2,TC5_IRQn},
	{TC2,0,TC6_IRQn},
	{TC2,1,TC7_IRQn},
	{TC2,2,TC8_IRQn}
};

Timer::Timer(int timerPeriod) : callback(NULL) {
	timerID = 0;
	// find the first available timer channel
	while(timerID < 9 && timerInstances[timerID] != NULL) timerID++;
	// when all timers are in use, re-use timer 8, execution is UNPREDICTABLE
	if(timerID == 9) {
		Serial.print("ERROR : No more timer available, re-using timer 8 !\n");
		timerID = 8;
	}
	timerInstances[timerID] = this;

	// Tell the Power Management Controller to disable
	// the write protection of the timer/Counter registers:
	pmc_set_writeprotect(false);
	// Enable clock for the timer
	pmc_enable_periph_clk((uint32_t)timerData[timerID].irq);
	// Set up the timer in waveform mode which creates a PWM
	// in UP mode with automatic trigger on RC Compare
	// and sets it up with the determined internal clock as clock input.
	TC_Configure(timerData[timerID].timer, timerData[timerID].channel,
		TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);
	// Enable the RC Compare Interrupt...
	timerData[timerID].timer->TC_CHANNEL[timerData[timerID].channel].TC_IER=TC_IER_CPCS;
	// ... and disable all others.
	timerData[timerID].timer->TC_CHANNEL[timerData[timerID].channel].TC_IDR=~TC_IER_CPCS;

	setPeriod(timerPeriod);
}

uint32_t Timer::getPeriod() {
	return period;
}
void Timer::setPeriod(uint32_t timerPeriod) {
	period = timerPeriod;

	// Reset counter and fire interrupt when RC value is matched:
	TC_SetRC(timerData[timerID].timer, timerData[timerID].channel, period);
}

// set the function to call when the timer expires, and the argument to pass to the callback
void Timer::setCallback(void (*cb)(void*), void* arg) {
	callback = cb;
	callbackArg = arg;
}

void Timer::start() {
	NVIC_ClearPendingIRQ(timerData[timerID].irq);
	NVIC_EnableIRQ(timerData[timerID].irq);

	TC_Start(timerData[timerID].timer, timerData[timerID].channel);
}
void Timer::stop() {
	NVIC_DisableIRQ(timerData[timerID].irq);

	TC_Stop(timerData[timerID].timer, timerData[timerID].channel);
}

// interrupt handlers for all timers
void TC0_Handler(void){
	TC_GetStatus(TC0, 0);
	if(timerInstances[0]->callback != NULL)
		timerInstances[0]->callback(timerInstances[0]->callbackArg);
}
void TC1_Handler(void){
	TC_GetStatus(TC0, 1);
	if(timerInstances[1]->callback != NULL)
		timerInstances[1]->callback(timerInstances[1]->callbackArg);
}
void TC2_Handler(void){
	TC_GetStatus(TC0, 2);
	if(timerInstances[2]->callback != NULL)
		timerInstances[2]->callback(timerInstances[2]->callbackArg);
}
void TC3_Handler(void){
	TC_GetStatus(TC1, 0);
	if(timerInstances[3]->callback != NULL)
		timerInstances[3]->callback(timerInstances[3]->callbackArg);
}
void TC4_Handler(void){
	TC_GetStatus(TC1, 1);
	if(timerInstances[4]->callback != NULL)
		timerInstances[4]->callback(timerInstances[4]->callbackArg);
}
void TC5_Handler(void){
	TC_GetStatus(TC1, 2);
	if(timerInstances[5]->callback != NULL)
		timerInstances[5]->callback(timerInstances[5]->callbackArg);
}
void TC6_Handler(void){
	TC_GetStatus(TC2, 0);
	if(timerInstances[6]->callback != NULL)
		timerInstances[6]->callback(timerInstances[6]->callbackArg);
}
void TC7_Handler(void){
	TC_GetStatus(TC2, 1);
	if(timerInstances[7]->callback != NULL)
		timerInstances[7]->callback(timerInstances[7]->callbackArg);
}
void TC8_Handler(void){
	TC_GetStatus(TC2, 2);
	if(timerInstances[8]->callback != NULL)
		timerInstances[8]->callback(timerInstances[8]->callbackArg);
}
