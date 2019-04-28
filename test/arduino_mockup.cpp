#include "arduino_mockup.h"

HardwareSerial::HardwareSerial() {

}

void HardwareSerial::begin(int baudrate) {
    (void)baudrate;
}

uint8_t HardwareSerial::read() {
    return 0;
}

void HardwareSerial::write(uint8_t data) {
    (void)data;
}
