#include "board.h"

#ifndef MOTOR_HPP
#define MOTOR_HPP

#define SIGN(a) (a > 0 ? 1 : -1)

// maximum speed (must be a power of 2, speed exceeding this value leads to an erronous behaviour)
#define MAX_SPEED 0x1000LL
// with integers, even a max_acceleration of 1 is too fast, so increment speed only on sub_speed overflow (> SUB_SPEED_MAX)
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
	int32_t position, break_position, goal_position;

	// limit acceleration by limiting speed change
	int16_t acceleration_limiter(int16_t speed) {
		if(abs(last_speed) <= min_speed) { // between -min_speed and min_speed, acceleration limiter is disengaged
			if(abs(speed) >= min_speed) {
				last_speed = min_speed*SIGN(speed);
			} else {
				last_speed = speed;
			}
		}

		if(speed - last_speed > max_speed_change) {
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

		digitalWrite(enable_pin, MOTOR_IDLE_LEVEL);
		digitalWrite(clock_pin, MOTOR_IDLE_LEVEL);
	}

	void update() {
		if((position & MOTOR_SUB_STEP_MASK) == (goal_position & MOTOR_SUB_STEP_MASK)) // goal position has been reached, stop
			target_speed = 0;
		else if(position >= break_position) // start breaking with speed > 0
			target_speed = min_speed;
		else if(position <= -break_position) // start breaking with speed < 0
			target_speed = -min_speed;

		int32_t delta_position = acceleration_limiter(target_speed);

		// send a tick when we completed a step
		if( ((position + delta_position) & MOTOR_SUB_STEP_MASK) != (position & MOTOR_SUB_STEP_MASK) ) {
			digitalWrite(dir_pin, (last_speed < 0) ^ inv_dir ? MOTOR_IDLE_LEVEL : MOTOR_ACTIVE_LEVEL);
			digitalWrite(clock_pin, MOTOR_ACTIVE_LEVEL);
		}
		position += delta_position;
	}

	void clear_pulse() {
		digitalWrite(dir_pin, (last_speed < 0) ^ inv_dir ? MOTOR_ACTIVE_LEVEL : MOTOR_IDLE_LEVEL);
		digitalWrite(clock_pin, MOTOR_IDLE_LEVEL);
	}

	void move(int16_t speed, int32_t distance) {
		position = sub_speed = 0;
		goal_position = distance;
		target_speed = abs(speed) > MAX_SPEED ? MAX_SPEED*SIGN(speed) : speed;

		// compute the position (abs value) where we will start to slow down
		break_position = (((uint32_t) target_speed)*((uint32_t) target_speed) - ((uint32_t) min_speed)*((uint32_t) min_speed))*MAX_SUB_SPEED/(2*max_acceleration);
		if(break_position > distance/2) // small move where we don't reach target speed
			break_position = abs(distance)/2;
		else
			break_position = abs(distance) - break_position;

		max_speed_change = max_acceleration/MAX_SUB_SPEED; // maximum speed delta from one iteration to the next
	}

	bool finished() { return target_speed == 0; }

	int16_t max_acceleration;
	uint16_t min_speed;
};

#endif
