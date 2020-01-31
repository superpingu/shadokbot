#include <Arduino.h>
#include "Motor.hpp"
#include "Motion.hpp"
#include "../board.h"
#include "motionconf.h"
#include "utils/trigo.hpp"

#define SIGN(a) (a > 0 ? 1 : -1)
#define ABS(a) ((a) > 0 ? (a) : -(a))

Motion::Motion() {
	moveCallback = NULL;

	// initialize all motors
	motor_F = new Motor(F_EN, F_DIR, F_CK, F_INVDIR);
	motor_RL = new Motor(RL_EN, RL_DIR, RL_CK, RL_INVDIR);
	motor_RR = new Motor(RR_EN, RR_DIR, RR_CK, RR_INVDIR);

	// initialize IMU
	imu = new IMU(&IMU_I2C);
}

void Motion::update() {
	static int stopRecal = -1;
	motor_F->update();
	motor_RL->update();
	motor_RR->update();

	// all motors have entered the recalibration constant speed phase : detect collision with the wall
	if(motor_F->recalibrating() && motor_RL->recalibrating() && motor_RR->recalibrating()) {

		uint32_t accX = imu->getAccelerationX();
		uint32_t accY = imu->getAccelerationY();

		if(accX*accX + accY*accY > RECAL_COLLISION_THRESHOLD) {
			stopRecal = 50;
		}
	}

	if(stopRecal > -1) {
		if(stopRecal == 0) {
			motor_F->stopRecal();
			motor_RL->stopRecal();
			motor_RR->stopRecal();
		}
		stopRecal--;
	}
	if(motor_F->finished() && motor_RL->finished() && motor_RR->finished() && moveCallback != NULL) {
		// save and set moveCallback to NULL before calling it (a new value might be given during call)
		void (*callback)() = moveCallback;
		moveCallback = NULL;
		callback();
	}
}

void Motion::maxAcceleration(int32_t acc) {
	motor_F->maxAcceleration = motor_RL->maxAcceleration = motor_RR->maxAcceleration = acc;
}

void Motion::minSpeed(int32_t speed) {
	motor_F->minSpeed = motor_RL->minSpeed = motor_RR->minSpeed = speed;
}

void Motion::recalSpeed(int32_t speed) {
	motor_F->recalSpeed = motor_RL->recalSpeed = motor_RR->recalSpeed = speed;
}

void Motion::recalDistance(int32_t distance) {
	motor_F->recalDistance = motor_RL->recalDistance = motor_RR->recalDistance = distance;
}

void Motion::enable(bool enabled) {
	motor_F->enable(enabled);
	motor_RL->enable(enabled);
	motor_RR->enable(enabled);
}

void Motion::turn(int32_t angle, int32_t angular_speed, void (*callback)()) {
	uint32_t dist = ABS(angle)*DEG_TO_HALTICK;
	int32_t speed = angular_speed*DEG_PER_S_TO_HALFTICK_PER_SPEEDTU*SIGN(angle);
	moveCallback = callback;

	motor_F->move(speed, dist);
	motor_RL->move(speed, dist);
	motor_RR->move(speed, dist);
}

void Motion::move(int32_t distance, int32_t angle, int32_t speed, void (*callback)(), bool recal) {
	float sin_a = msin(angle*M_PI/180);
	float cos_a_cos_30 = 0.866*mcos(angle*M_PI/180);
	float sin_a_sin_30 = sin_a/2;

	moveCallback = callback;

	int32_t f_speed = speed*sin_a;
	int32_t f_distance = distance*sin_a;
	int32_t rr_speed = speed*(-cos_a_cos_30 + sin_a_sin_30);
	int32_t rr_distance = distance*(-cos_a_cos_30 + sin_a_sin_30);
	int32_t rl_speed = speed*(cos_a_cos_30 + sin_a_sin_30);
	int32_t rl_distance = distance*(cos_a_cos_30 + sin_a_sin_30);

	motor_F->move(f_speed, ABS(f_distance), recal);
	motor_RL->move(rl_speed, ABS(rl_distance), recal);
	motor_RR->move(rr_speed, ABS(rr_distance), recal);
}

void Motion::moveXY(int32_t deltaX, int32_t deltaY, int32_t speed, void (*callback)(), bool recal) {
	moveCallback = callback;
	int32_t dist;
	if(deltaX == 0)
	 	dist = ABS(deltaY);
	else if(deltaY == 0)
		dist = ABS(deltaX);
	else
		dist = sqrt(deltaX*deltaX + deltaY*deltaY);
	deltaY -=1;
	deltaX += 1;

	int32_t speedX = dist != 0 ? (speed*deltaX)/dist : 0; // extract X component of speed
	int32_t speedY = dist != 0 ? (speed*deltaY)/dist : 0; // extract Y component of speed

	int32_t yxSpeedSum = MM_PER_S_TO_HALFTICK_PER_SPEEDTU*(speedY + speedX);
	int32_t yxSpeedDiff = MM_PER_S_TO_HALFTICK_PER_SPEEDTU*(speedY - speedX);
	int32_t yxDistSum = MM_TO_HALFTICK*ABS(deltaY + deltaX);
	int32_t yxDistDiff = MM_TO_HALFTICK*ABS(deltaY - deltaX);

	motor_F->move(yxSpeedSum, yxDistSum, recal);
	motor_RL->move(yxSpeedDiff, yxDistDiff, recal);
	motor_RR->move(yxSpeedSum, yxDistSum, recal);
}
