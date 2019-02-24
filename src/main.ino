#include "board.h"
#include "shell/Shell.hpp"
#include "shell/commands.h"
#include "ax12/AX12.hpp"

Shell* sh;
AX12* a;

void lol() {
	static int pos = 400;
	Serial.print(a->getVoltage());
	Serial.print("\n");
	a->move(pos, lol);
	pos = 1024 - pos;
}

// the setup function runs once when you press reset or power the board
void setup() {
	sh = new Shell(115200, getComms());

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);
	AX12::init(&Serial1, 115200);
	a = new AX12(1);
	a->setSpeed(500);
	a->move(600, lol);

	//motion = new Motion();
	//motion->enable(true);
}

// the loop function runs over and over again forever
void loop() {
	//motion->update();
	sh->update();
	AX12::update();
	delay(10);
}
