#include "gnuplot-iostream/gnuplot-iostream.h"

#define HIGH 1
#define LOW 0
#define OUTPUT 0

#include "../board.h"

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
	motor.min_speed = 100;
	motor.max_acceleration = 1000;
	motor.move(-1600, 1200*4096);

	for (size_t i = 0; i < 15000; i++) {
		motor.update();
		motor.clearPulse();
		pts.push_back(std::pair<double, double>(i, step));
		if(motor.finished() && step < 1000)
			motor.move(-800, 800*4096);
		std::cout << step << '\n';
	}

	gp << "plot '-' with lines\n";
	gp.send1d(pts);

	return 0;
}
