#ifndef ARDUINO_PIN_H
#define ARDUINO_PIN_H

#define HIGH 255
#define LOW 0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

void digitalWrite(int pin, int value);
int digitalRead(int pin);
void pinMode(int pin, int type);
int analogRead(int pin);

#endif /* ARDUINO_PIN_H */
