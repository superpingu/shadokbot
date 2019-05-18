/* IMU (Inertial Motion Unit) driver for bno055
 * keeps track of the robot's relative orientation and acceleration
 * functionnalities :
 *  > get linear acceleration
 *  > get gravity vector
 *  > get heading, roll and pitch
 */

#ifndef IMU_HPP
#define IMU_HPP

#include <Arduino.h>
#include <Wire.h>

class IMU {
private:
	TwoWire* i2cPort; // IMU I2C port to use (there are 2 on the Due)

	// IMU register access functions
	void write8(uint8_t addr, uint8_t value);
	uint8_t read8(uint8_t addr);
	uint16_t read16(uint8_t lsb_addr);
public:
	// Initialize the bno055 and set it in IMU mode (fusion between accelerometer and gyro).
	IMU(TwoWire* port);

	// Get the heading angle, relative to the IMU start-up.
	// The range is [0, 5760]. Value increases when turning clockwise.
	int16_t getHeading(void);
	// Get the pitch angle relative to the IMU start-up.
	// The output range is [-2880, 2880].
	int16_t getPitch(void);
	// Get the roll angle relative to the IMU start-up.
	// The output range is [-1440, 1440]. The value increases as the
	// inclination increases.
	int16_t getRoll(void);

	int16_t getAccelerationX();
	int16_t getAccelerationY();
	int16_t getAccelerationZ();

	int16_t getGravityVectorX();
	int16_t getGravityVectorY();
	int16_t getGravityVectorZ();

	int16_t getLinearAccelerationX();
	int16_t getLinearAccelerationY();
	int16_t getLinearAccelerationZ();
};

#endif
