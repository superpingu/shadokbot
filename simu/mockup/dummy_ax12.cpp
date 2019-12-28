#include "ax12/AX12.hpp"
#include "AXcomms.hpp"

AXcomms::AXcomms(uint8_t axid) {}
void AXcomms::init(HardwareSerial* serialport, int baudrate) {}

void AX12::update() {}
AX12::AX12(uint8_t axid) : AXcomms(axid) {}

int AX12::getPosition() {return 0;}
int AX12::getSpeed() {return 0;}
int AX12::getLoad() {return 0;}
int AX12::getStatus() {return 0;}
int AX12::getVoltage() {return 0;}
int AX12::getTemperature() {return 0;}
bool AX12::isMoving() {return 0;}

int AX12::setMode(AX12mode mode) {return 0;}
int AX12::setSpeed(int speed) {return 0;}
int AX12::setTorque(int torque) {return 0;}
int AX12::setLED(bool state) {return 0;}
int AX12::move(int position, void (*callback)(void)) {return 0;}
void AX12::cancelCallback() {}
int AX12::turn(double speed) {return 0;}
