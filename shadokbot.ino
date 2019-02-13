#include "board.h"
#include "Motor.hpp"
#include "RobotMotion.hpp"
#include "Shell.hpp"
#include "robotshell.hpp"

RobotMotion* motion;
Shell* sh;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	motion = new RobotMotion();
	motion->enable(true);
	
	sh = initCommands(motion);
}

// the loop function runs over and over again forever
void loop() {
	motion->update();
	sh->update();
}
