#include "gnuplot-iostream/gnuplot-iostream.h"
#include "../board.h"

#define HIGH 1
#define LOW 1
#define OUTPUT 0

int step = 0;

void pinMode(int pin, int mode) {}
void digitalWrite(int pin, int value) {
	if(pin == 3 && value == MOTOR_ACTIVE_LEVEL) {
		step++;
	}
}

#include "../Motor.hpp"

int main() {
	Gnuplot gp;
	std::vector< std::pair<double, double> > pts;

	Motor motor = Motor(1, 2, 3, false);
	motor.min_speed = 0;
	motor.max_acceleration = 1000;
	motor.move(400, 800*4096);

	for (size_t i = 0; i < 10000; i++) {
		motor.update();
		//motor.clear_pulse();
		pts.push_back(std::pair<double, double>(i, step/2));
	}

	gp << "plot '-' with lines\n";
	gp.send1d(pts);

	return 0;
}
