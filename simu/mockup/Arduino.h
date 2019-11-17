#ifndef ARDUINO_H
#define ARDUINO_H

#include "serial.hpp"
extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;

#include "arduino_time.hpp"
#include "arduino_pin.hpp"
#include <math.h>

#include "Wire.h"

extern TwoWire Wire;

void setup();
void loop();

#endif /* ARDUINO_H */
