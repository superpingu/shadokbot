#ifndef AXCOMMS_HPP
#define AXCOMMS_HPP

#include "Arduino.h"

class AXcomms {
public:
    AXcomms(uint8_t axid);
    static void init(HardwareSerial* serialport, int baudrate);
};

#endif /* AXCOMMS_HPP */
