#ifndef ARDUINO_H
#define ARDUINO_H

#define HIGH 1
#define LOW 0
#define OUTPUT 0

int pins[16]; // contains to current state of the arduino pins
void (*delay_callback)() = NULL; // called when an arduino delay is called

// Arduino functions mockup
void pinMode(int pin, int mode) {}
void delayMicroseconds(int delay) { if(delay_callback != NULL) delay_callback(); }
unsigned long micros() { return 0; }
void digitalWrite(int pin, int value) { if(pin < 16) pins[pin] = value; }

#endif
