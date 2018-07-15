#ifndef ROBOTMOTION_H
#define ROBOTMOTION_H

#include "Motor.hpp"

class RobotMotion {
private:
	Motor *motor_FL, *motor_FR, *motor_RL, *motor_RR;

	int16_t max_acceleration;
	uint16_t min_speed;

	void (*move_finished)(); // called when a move is finished
public:
	RobotMotion() {
		// initialize all motors
		motor_FL = new Motor(FL_EN, FL_DIR, FL_CK, FL_INV);
		motor_FR = new Motor(FR_EN, FR_DIR, FR_CK, FR_INV);
		motor_RL = new Motor(RL_EN, RL_DIR, RL_CK, RL_INV);
		motor_RR = new Motor(RR_EN, RR_DIR, RR_CK, RR_INV);

		// setup default motion parameters
		maxAcceleration(DEFAULT_MAX_ACCELERATION);
		minSpeed(DEFAULT_MIN_SPEED);

		move_finished = NULL;
	}

	// send ticks to the stepper motors at the appropriate frenquency
	void update() {
		unsigned long start = micros();
		motor_FL->update();
		motor_FR->update();
		motor_RL->update();
		motor_RR->update();

		delayMicroseconds(MOTORLOOP_HALFPERIOD - 50); // compensate the computation time

		motor_FL->clearPulse();
		motor_FR->clearPulse();
		motor_RL->clearPulse();
		motor_RR->clearPulse();

		if(motor_FL->finished() && motor_FR->finished() && motor_RL->finished() && motor_RR->finished() && move_finished != NULL)
			move_finished();

		unsigned long loop_time = micros() - start;
		delayMicroseconds(MOTORLOOP_HALFPERIOD*2 - loop_time);
	}


	// get or set maximum allowed acceleration for the robot
	int16_t maxAcceleration(int16_t acc = 0) {
		max_acceleration = acc == 0 ? max_acceleration : acc;
		motor_FL->max_acceleration = motor_FR->max_acceleration = acc;
		motor_RL->max_acceleration = motor_RR->max_acceleration = acc;
		return max_acceleration;
	}

	// get or set minimum speed of the robot
	uint16_t minSpeed(uint16_t speed = 0) {
		min_speed = speed == 0 ? min_speed : speed;
		motor_FL->min_speed = motor_FR->min_speed = speed;
		motor_RL->min_speed = motor_RR->min_speed = speed;
		return min_speed;
	}

	void moveFinished(void (*func)()) {
		move_finished = func;
	}

	void turn(int16_t angle, uint16_t angular_speed) {
		uint32_t dist = ABS(angle)*DEG_TO_SUBSTEP;
		uint16_t speed = speed*DEG_PER_S_TO_SUBSTEP_PER_PERIOD*SIGN(angle);

		motor_FL->move(-speed, dist);
		motor_FR->move(speed, dist);
		motor_RL->move(-speed, dist);
		motor_RR->move(speed, dist);
	}
};

#endif
