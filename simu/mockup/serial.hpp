#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>

class HardwareSerial {
public:
    HardwareSerial();
    void print(int data);
    void print(const char* data);
    void write(uint8_t data);
    bool available();
    char read();
    void begin(int baudrate);
    void setTimeout(int timeout);
    void flush();
    void println(int i);
};

#endif /* SERIAL_HPP */
