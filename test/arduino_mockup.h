#ifndef ARDUINO_MOCKUP
#define ARDUINO_MOCKUP

#include <stdint.h>

class HardwareSerial {
public:
    HardwareSerial();
    void begin(int baudrate);
    uint8_t read();
    void write(uint8_t data);
};

#endif
