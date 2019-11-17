#include "arduino_pin.hpp"
#include "stdio.h"
int pins[55] = {HIGH};

void digitalWrite(int pin, int value)
{
	pins[pin] = value;
}

int digitalRead(int pin) {
    return pins[pin];
}

void pinMode(int pin, int type) {

}

int analogRead(int pin) {
    return 0;
}
