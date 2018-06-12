#include "board.h"
#include "Motor.hpp"

#define MOTORLOOP_HALFPERIOD 125 // us

Motor *motor_FL, *motor_FR, *motor_RL, *motor_RR;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	// initialize all motors
	motor_FL = new Motor(FL_EN, FL_DIR, FL_CK, FL_INV);
	motor_FR = new Motor(FR_EN, FR_DIR, FR_CK, FR_INV);
	motor_RL = new Motor(RL_EN, RL_DIR, RL_CK, RL_INV);
	motor_RR = new Motor(RR_EN, RR_DIR, RR_CK, RR_INV);
	motor_FL->max_acceleration = motor_FR->max_acceleration = motor_RL->max_acceleration = motor_RR->max_acceleration = 800;
	motor_FL->min_speed = motor_FR->min_speed = motor_RL->min_speed = motor_RR->min_speed = 100;

	int32_t dist = 800LL*4096LL;
	motor_FL->move(400, dist);
	motor_FR->move(400, dist);
	motor_RL->move(400, dist);
	motor_RR->move(400, dist);
}

// the loop function runs over and over again forever
void loop() {
  update_motors();
}

void update_motors() {
	unsigned long start = micros();
	motor_FL->update();
	motor_FR->update();
	motor_RL->update();
	motor_RR->update();

	delayMicroseconds(MOTORLOOP_HALFPERIOD - 50); // compensate the computation time

	motor_FL->clear_pulse();
	motor_FR->clear_pulse();
	motor_RL->clear_pulse();
	motor_RR->clear_pulse();

	if(motor_FL->finished() && motor_FR->finished() && motor_RL->finished() && motor_RR->finished()) {
		move_finished();
	}

	unsigned long loop_time = micros() - start;
	delayMicroseconds(MOTORLOOP_HALFPERIOD*2 - loop_time);
}

void move_finished() {

}
