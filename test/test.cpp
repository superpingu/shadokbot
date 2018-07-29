#include "gnuplot-iostream/gnuplot-iostream.h"

#include "arduino.h"
#include "../board.h"
#include "../RobotMotion.hpp"
#include "stepper.hpp"

stepper_motor fl_stepper(FL_EN, FL_DIR, FL_CK, FL_INV);
stepper_motor fr_stepper(FR_EN, FR_DIR, FR_CK, FR_INV);
stepper_motor rl_stepper(RL_EN, RL_DIR, RL_CK, RL_INV);
stepper_motor rr_stepper(RR_EN, RR_DIR, RR_CK, RR_INV);

void updateMotors() {
	fl_stepper.update();
	fr_stepper.update();
	rl_stepper.update();
	rr_stepper.update();
}

int main() {
	Gnuplot gp;

	delay_callback = updateMotors;
	RobotMotion motion;
	motion.move(200, 135, 200);

	for(int t = 0; t < 6000; t++) {
		motion.update();
		fl_stepper.log(t);
		fr_stepper.log(t);
		rl_stepper.log(t);
		rr_stepper.log(t);
	}

	gp << "set multiplot layout 2,2 rowsfirst\n";
	gp << "plot '-' with lines lc rgb \"blue\"\n";
	gp.send1d(fl_stepper.speeds);
	gp << "plot '-' with lines lc rgb \"red\"\n";
	gp.send1d(fr_stepper.speeds);
	gp << "plot '-' with lines lc rgb \"green\"\n";
	gp.send1d(rl_stepper.speeds);
	gp << "plot '-' with lines lc rgb \"orange\"\n";
	gp.send1d(rr_stepper.speeds);

	return 0;
}
