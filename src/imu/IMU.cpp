#include "IMU.hpp"
#include "bno055.h"

#define SYS_TRIGGER_RST_SYS 0x20U /* Reset the device */
#define SYS_TRIGGER_INTERNAL_OSC 0x00 /* Use internal oscillator */

void IMU::write8(uint8_t addr, uint8_t value) {
	i2cPort->beginTransmission(BNO055_ADDRESS);
	i2cPort->write(addr);
	i2cPort->write(value);
	i2cPort->endTransmission();
}
uint8_t IMU::read8(uint8_t addr) {
	i2cPort->beginTransmission(BNO055_ADDRESS);
	i2cPort->write(addr);
	i2cPort->endTransmission();
	i2cPort->requestFrom(BNO055_ADDRESS, 1);
	return i2cPort->read();
}
uint16_t IMU::read16(uint8_t lsb_addr) {
	uint16_t value;
	i2cPort->beginTransmission(BNO055_ADDRESS);
	i2cPort->write(lsb_addr);
	i2cPort->endTransmission();
	i2cPort->requestFrom(BNO055_ADDRESS, 2);
	value = i2cPort->read();
	value += i2cPort->read() << 8;
	return value;
}

IMU::IMU(TwoWire* port) {
	i2cPort = port;
	i2cPort->begin();

	// Make sure we have the right device
	if(read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
		Serial.print("IMU : wrong ID, abort init");
		return;
	}

	// Switch to config mode (just in case since this is the default)
	write8(BNO055_OPR_MODE_ADDR, OPERATION_MODE_CONFIG);
	delay(30);

	// Reset
	write8(BNO055_SYS_TRIGGER_ADDR, SYS_TRIGGER_RST_SYS);
	delay(200);
	while(read8(BNO055_CHIP_ID_ADDR) != BNO055_ID)
		delay(10);
	delay(50);

	// Set to normal power mode
	write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
	delay(10);
	// why ??
	write8(BNO055_PAGE_ID_ADDR, 0);
	// why ??
	write8(BNO055_SYS_TRIGGER_ADDR, SYS_TRIGGER_INTERNAL_OSC);
	delay(10);

	// euler angles in deg, acceleration in mg
	write8(BNO055_UNIT_SEL_ADDR, 0x80);
	delay(10);

	// Set the operating mode (see section 3.3)
	write8(BNO055_OPR_MODE_ADDR, OPERATION_MODE_IMUPLUS);
	delay(50);
}

int16_t IMU::getHeading() { return read16(BNO055_EULER_H_LSB_ADDR); }
int16_t IMU::getPitch() { return read16(BNO055_EULER_P_LSB_ADDR); }
int16_t IMU::getRoll() { return read16(BNO055_EULER_R_LSB_ADDR); }

int16_t IMU::getAccelerationX() { return read16(BNO055_ACCEL_DATA_X_LSB_ADDR); }
int16_t IMU::getAccelerationY() { return read16(BNO055_ACCEL_DATA_Y_LSB_ADDR); }
int16_t IMU::getAccelerationZ() { return read16(BNO055_ACCEL_DATA_Z_LSB_ADDR); }

int16_t IMU::getGravityVectorX() { return read16(BNO055_GRAVITY_DATA_X_LSB_ADDR); }
int16_t IMU::getGravityVectorY() { return read16(BNO055_GRAVITY_DATA_Y_LSB_ADDR); }
int16_t IMU::getGravityVectorZ() { return read16(BNO055_GRAVITY_DATA_Z_LSB_ADDR); }

int16_t IMU::getLinearAccelerationX() { return read16(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR); }
int16_t IMU::getLinearAccelerationY() { return read16(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR); }
int16_t IMU::getLinearAccelerationZ() { return read16(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR); }
