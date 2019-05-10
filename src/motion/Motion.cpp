#include "Arduino.h"
#include "Motor.hpp"
#include "Motion.hpp"
#include "../board.h"
#include "motionconf.h"

#define SIGN(a) (a > 0 ? 1 : -1)
#define ABS(a) ((a) > 0 ? (a) : -(a))

Motion* motion; // pointer to motion instance

Motion::Motion() {
	moveCallback = NULL;

	// initialize all motors
	motor_FL = new Motor(FL_EN, FL_DIR, FL_CK, FL_INVDIR);
	motor_FR = new Motor(FR_EN, FR_DIR, FR_CK, FR_INVDIR);
	motor_RL = new Motor(RL_EN, RL_DIR, RL_CK, RL_INVDIR);
	motor_RR = new Motor(RR_EN, RR_DIR, RR_CK, RR_INVDIR);

	// initialize IMU
	imu = new IMU(&IMU_I2C);
}

void Motion::update() {
	motor_FL->update();
	motor_FR->update();
	motor_RL->update();
	motor_RR->update();

	// all motors have entered the recalibration constant speed phase : detect collision with the wall
	if(motor_FL->recalibrating() && motor_FR->recalibrating() && motor_RL->recalibrating() && motor_RR->recalibrating()) {
		uint32_t accX = imu->getLinearAccelerationX();
		uint32_t accY = imu->getLinearAccelerationY();

		if(accX*accX + accY*accY > RECAL_COLLISION_THRESHOLD) {
			motor_FL->stopRecal();
			motor_FR->stopRecal();
			motor_RL->stopRecal();
			motor_RR->stopRecal();
		}
	}

	if(motor_FL->finished() && motor_FR->finished() && motor_RL->finished() && motor_RR->finished() && moveCallback != NULL) {
		// save and set moveCallback to NULL before calling it (a new value might be given during call)
		void (*callback)() = moveCallback;
		moveCallback = NULL;
		callback();
	}
}

void Motion::maxAcceleration(int32_t acc) {
	motor_FL->maxAcceleration = motor_FR->maxAcceleration = acc;
	motor_RL->maxAcceleration = motor_RR->maxAcceleration = acc;
}

void Motion::minSpeed(int32_t speed) {
	motor_FL->minSpeed = motor_FR->minSpeed = speed;
	motor_RL->minSpeed = motor_RR->minSpeed = speed;
}

void Motion::recalSpeed(int32_t speed) {
	motor_FL->recalSpeed = motor_FR->recalSpeed = speed;
	motor_RL->recalSpeed = motor_RR->recalSpeed = speed;
}

void Motion::recalDistance(int32_t distance) {
	motor_FL->recalDistance = motor_FR->recalDistance = distance;
	motor_RL->recalDistance = motor_RR->recalDistance = distance;
}

void Motion::enable(bool enabled) {
	motor_FL->enable(enabled);
	motor_FR->enable(enabled);
	motor_RL->enable(enabled);
	motor_RR->enable(enabled);
}

void Motion::turn(int32_t angle, int32_t angular_speed, void (*callback)()) {
	uint32_t dist = ABS(angle)*DEG_TO_HALTICK;
	int32_t speed = angular_speed*DEG_PER_S_TO_HALFTICK_PER_SPEEDTU*SIGN(angle);
	moveCallback = callback;

	motor_FL->move(-speed, dist);
	motor_FR->move(speed, dist);
	motor_RL->move(-speed, dist);
	motor_RR->move(speed, dist);
}

void Motion::move(int32_t distance, int32_t angle, int32_t speed, void (*callback)(), bool recal) {
	float y_coeff = cos(angle*M_PI/180);
	float x_coeff = sin(angle*M_PI/180);
	moveCallback = callback;

	int32_t yx_speed_sum = speed*MM_PER_S_TO_HALFTICK_PER_SPEEDTU*(y_coeff + x_coeff);
	int32_t yx_speed_diff = speed*MM_PER_S_TO_HALFTICK_PER_SPEEDTU*(y_coeff - x_coeff);
	int32_t yx_dist_sum = distance*MM_TO_HALFTICK*ABS(y_coeff + x_coeff);
	int32_t yx_dist_diff = distance*MM_TO_HALFTICK*ABS(y_coeff - x_coeff);

	motor_FL->move(yx_speed_diff, ABS(yx_dist_diff), recal);
	motor_FR->move(yx_speed_sum, ABS(yx_dist_sum), recal);
	motor_RL->move(yx_speed_sum, ABS(yx_dist_sum), recal);
	motor_RR->move(yx_speed_diff, ABS(yx_dist_diff), recal);
}
