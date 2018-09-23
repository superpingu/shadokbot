#include "gnuplot-iostream/gnuplot-iostream.h"
#include <chrono>
#include <thread>

#include "arduino.h"
#include "../board.h"
#include "../RobotMotion.hpp"
#include "stepper.hpp"
#include "../Shell.hpp"
#include "../robotshell.hpp"

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

float angle = 0; // robot heading angle in deg
float x_pos = 0; // robot x position in mm
float y_pos = 0; // robot y position in mm

void position_tracking() {
	static int last_fl_step = 0, last_fr_step = 0, last_rl_step = 0, last_rr_step = 0;

	float delta_y = ((fl_stepper.step() - last_fl_step) + (fr_stepper.step() - last_fr_step)
		+ (rl_stepper.step() - last_rl_step) + (rr_stepper.step() - last_rr_step))*MAX_SPEED/4.0/MM_TO_SUBSTEP;

	float delta_x = ((fl_stepper.step() - last_fl_step) - (fr_stepper.step() - last_fr_step)
		- (rl_stepper.step() - last_rl_step) + (rr_stepper.step() - last_rr_step))*MAX_SPEED/4.0/MM_TO_SUBSTEP;

	float delta_angle = (-(fl_stepper.step() - last_fl_step) + (fr_stepper.step() - last_fr_step)
		- (rl_stepper.step() - last_rl_step) + (rr_stepper.step() - last_rr_step))*MAX_SPEED/4.0/DEG_TO_SUBSTEP;

	angle += delta_angle;
	x_pos += delta_x*cos(angle*M_PI/180) + delta_y*sin(angle*M_PI/180);
	y_pos += -delta_x*sin(angle*M_PI/180) + delta_y*cos(angle*M_PI/180);

	last_fl_step = fl_stepper.step();
	last_fr_step = fr_stepper.step();
	last_rl_step = rl_stepper.step();
	last_rr_step = rr_stepper.step();
}

RobotMotion* motion;

void next_move() {
	static int state = 0;
	switch (state) {
	case 0:
		motion->move(300, 45, 200);
		break;
	case 1:
		motion->move(300, -45, 200);
		break;
	case 2:
		motion->move(300, -135, 200);
		break;
	case 3:
		motion->move(300, 135, 200);
		break;
	}
	state++;
}

int main() {
	Gnuplot gp;
	std::vector<std::pair<float, float> > positions;
	positions.push_back(std::pair<float, float>(0, 0));
	delay_callback = updateMotors;

	motion = new RobotMotion();
	Shell* sh = initCommands(motion);

	bool moved = false;
	uint32_t t = 0;

	while(true) {
		motion->update();
		sh->update();

		position_tracking();
		//fl_stepper.log(t);
		//fr_stepper.log(t);
		//rl_stepper.log(t);
		//rr_stepper.log(t);

		if(x_pos != positions.back().first || y_pos != positions.back().second) {
			positions.push_back(std::pair<float, float>(x_pos, y_pos));
			moved = true;
		}
		if((t % 100) == 99 && moved) {
			gp << "plot '-' with lines lc rgb \"orange\"\n";
			gp.send1d(positions);
			moved = false;
		}

		std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
		t++;
	}

	//gp << "set multiplot layout 2,2 rowsfirst\n";
	//gp << "plot '-' with lines lc rgb \"blue\"\n";
	//gp.send1d(fl_stepper.speeds);
	//gp << "plot '-' with lines lc rgb \"red\"\n";
	//gp.send1d(fr_stepper.speeds);
	//gp << "plot '-' with lines lc rgb \"green\"\n";
	//gp.send1d(rl_stepper.speeds);
	//gp << "plot '-' with lines lc rgb \"orange\"\n";
	//gp.send1d(rr_stepper.speeds);

	gp << "plot '-' with lines lc rgb \"orange\"\n";
	gp.send1d(positions);

	return 0;
}
