#include "arduino_time.hpp"
#include <time.h>
#include <unistd.h>
#include "simu_time.hpp"

unsigned long micros()
{
    return Time::getCurTime() * 1000;
}

void delayMicroseconds(unsigned int delay)
{
    usleep(delay);
}
