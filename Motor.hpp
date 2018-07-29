#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "board.h"
#include "robotconf.h"

#define SIGN(a) (a > 0 ? 1 : -1)
#define ABS(a) (a > 0 ? a : -a)

// maximum speed (must be a power of 2, speed exceeding this value leads to an erronous behaviour)
#define MAX_SPEED 0x1000LL
// with integers, even a max_acceleration of 1 is too fast, so increment speed only on sub_speed overflow (> MAX_SUB_SPEED)
#define MAX_SUB_SPEED ((int16_t) 0x1000LL)

// to allow speed control, position is incremented by a small value at each update,
// MOTOR_SUB_STEP_MASK masks out the bits used for substep increment
#define MOTOR_SUB_STEP_MASK (((uint32_t) 0xFFFFFFFF) - ((uint32_t) (MAX_SPEED-1)))

class Motor {
private:
 	const uint8_t enable_pin, clock_pin, dir_pin;
	const bool inv_dir;
	int16_t target_speed, last_speed;
	int16_t sub_speed, max_speed_change;
	uint32_t position, break_position, goal_position;

	// limit acceleration by limiting speed change
	int16_t accelerationLimiter(int16_t speed) {
		if(ABS(last_speed) <= min_speed) { // between -min_speed and min_speed, acceleration limiter is disengaged
			last_speed = ABS(speed) > min_speed ? (min_speed+1)*SIGN(speed) : speed;
		} else if(speed - last_speed > max_speed_change) {
			sub_speed += max_acceleration;
			while(sub_speed >= MAX_SUB_SPEED) {
				sub_speed -= MAX_SUB_SPEED;
				last_speed += 1;
			}
		} else if(speed - last_speed < -max_speed_change) {
			sub_speed += max_acceleration;
			while(sub_speed >= MAX_SUB_SPEED) {
				sub_speed -= MAX_SUB_SPEED;
				last_speed -= 1;
			}
		} else {
			last_speed = speed;
		}
		return last_speed;
	}

public:
	Motor(uint8_t _enable_pin, uint8_t _dir_pin, uint8_t _clock_pin, bool _inv_dir) : enable_pin(_enable_pin), clock_pin(_clock_pin), dir_pin(_dir_pin), inv_dir(_inv_dir)
	{
		target_speed = last_speed = sub_speed = 0;
		goal_position = position = 0;

		pinMode(enable_pin, OUTPUT);
		pinMode(dir_pin, OUTPUT);
		pinMode(clock_pin, OUTPUT);

		digitalWrite(enable_pin, MOTOR_ACTIVE_LEVEL);
		digitalWrite(clock_pin, MOTOR_IDLE_LEVEL);
	}

	void update() {
		if(position >= goal_position) // goal position has been reached, stop
			target_speed = 0;
		else if(position >= break_position) // start breaking
			target_speed = min_speed*SIGN(last_speed);

		accelerationLimiter(target_speed);

		// send a tick when we completed a step
		if((position & (MAX_SUB_SPEED - 1)) + ABS(last_speed) >= MAX_SUB_SPEED) {
			digitalWrite(dir_pin, (last_speed < 0) ^ inv_dir ? MOTOR_IDLE_LEVEL : MOTOR_ACTIVE_LEVEL);
			digitalWrite(clock_pin, MOTOR_ACTIVE_LEVEL);
		}
		position += ABS(last_speed);
	}

	void clearPulse() {
		digitalWrite(dir_pin, (last_speed < 0) ^ inv_dir ? MOTOR_ACTIVE_LEVEL : MOTOR_IDLE_LEVEL);
		digitalWrite(clock_pin, MOTOR_IDLE_LEVEL);
	}

	void move(int16_t speed, int32_t distance) {
		digitalWrite(enable_pin, MOTOR_IDLE_LEVEL); // enable motors on first move

		position = 0;
		sub_speed = 0;
		goal_position = ABS(distance);// + (speed < 0 ? MAX_SPEED : 0);
		target_speed = ABS(speed) > MAX_SPEED ? MAX_SPEED*SIGN(speed) : (ABS(speed) < min_speed ? min_speed*SIGN(speed) : speed);

		// compute the position (abs value) where we will start to slow down
		break_position = (((int32_t) target_speed)*((int32_t) target_speed) - ((int32_t) min_speed)*((int32_t) min_speed))/(2*max_acceleration);
		break_position = break_position*MAX_SUB_SPEED; // computation in 2 steps to avoid int overflow
		if(break_position > goal_position/2) // small move where we don't reach target speed
			break_position = goal_position/2;
		else
			break_position = goal_position - break_position;

		max_speed_change = max_acceleration/MAX_SUB_SPEED; // maximum speed delta from one iteration to the next
	}

	bool finished() { return position >= goal_position; }

	int16_t max_acceleration;
	int16_t min_speed;
};

#endif
