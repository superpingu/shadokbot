#include "board.h"
#include "Motor.hpp"
#include "RobotMotion.hpp"

RobotMotion* motion;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	motion = new RobotMotion();

	motion->turn(180, 0);
}

// the loop function runs over and over again forever
void loop() {
 motion->update();
}
