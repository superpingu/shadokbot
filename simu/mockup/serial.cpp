#include "serial.hpp"
#include <stdio.h>

HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;

HardwareSerial::HardwareSerial()
{
}

void HardwareSerial::print(int data)
{
    printf("%d\n", data);
}

void HardwareSerial::print(const char *data)
{
    printf("%s\n", data);
}

void HardwareSerial::write(uint8_t data)
{
    // Nothing to do
}

bool HardwareSerial::available()
{
    return 0;
}

char HardwareSerial::read()
{
    return '\0';
}

void HardwareSerial::begin(int baudrate)
{
    // Nothing to do
}

void HardwareSerial::setTimeout(int timeout)
{
    // Nothing to do
}

void HardwareSerial::flush()
{
    // Nothing to do
}

void HardwareSerial::println(int i)
{
    // Nothing to do
}
