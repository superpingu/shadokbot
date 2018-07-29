#ifndef STEPPER_HPP
#define STEPPER_HPP

#include "arduino.h"

// speed filter parameters
#define FILTER_ORDER 16
#define FILTER_COEFF 0.92

class iir_filter {
	const double coeff;
	const unsigned int order;
	double last;
	iir_filter* subfilter;
public:
	iir_filter(double _coeff, unsigned int _order) : coeff(_coeff), order(_order) {
		if(order > 1)
		subfilter = new iir_filter(coeff, order-1);
	}
	double operator()(double new_val) {
		last = last*coeff + (1-coeff)*new_val;
		return order > 1 ? (*subfilter)(last) : last;
	}
};

class stepper_motor {
	const uint8_t enable_pin, clock_pin, dir_pin;
	const bool inv_dir;

	iir_filter filter;
	int stepper_speed;
	int stepper_step;
	int dir;
	int last_clk;

public:
	std::vector< std::pair<double, double> > positions, speeds;

	stepper_motor(uint8_t _enable_pin, uint8_t _dir_pin, uint8_t _clock_pin, bool _inv_dir) :
		enable_pin(_enable_pin), clock_pin(_clock_pin), dir_pin(_dir_pin), inv_dir(_inv_dir), filter(FILTER_COEFF, FILTER_ORDER) {}

	void update() {
		// rising edge on the clock pin
		if(pins[clock_pin] == MOTOR_ACTIVE_LEVEL && last_clk == MOTOR_IDLE_LEVEL) {
			dir = (pins[dir_pin] == MOTOR_ACTIVE_LEVEL) ^ inv_dir ? 1 : -1;
			stepper_step += dir;
			stepper_speed = dir*MAX_SPEED;
		}
		last_clk = pins[clock_pin];
	}

	double speed() {
		double res = filter(stepper_speed);
		stepper_speed = 0;
		return res;
	}
	double step() {
		return stepper_step;
	}

	void log(int index) {
		positions.push_back( std::pair<double, double>(index, step()) );
		speeds.push_back( std::pair<double, double>(index, speed()) );
	}
};

#endif
