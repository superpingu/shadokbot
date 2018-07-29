#include "board.h"
#include "Motor.hpp"
#include "RobotMotion.hpp"

RobotMotion* motion;

void s3() {
	motion->move(200, 360-45, 200);
	motion->moveFinished(NULL);
}

void s2() {
	motion->move(200, 225, 200);
	motion->moveFinished(s3);
}

void s1() {
	motion->move(200, 135, 200);
	motion->moveFinished(s2);
}

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	motion = new RobotMotion();
	motion->move(200, 45, 200);
	motion->moveFinished(s1);
}

// the loop function runs over and over again forever
void loop() {
	motion->update();
}
