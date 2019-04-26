
#include "shell/Shell.hpp"
#include "ax12/ax-constants.h"
#include "ax12/AX12.hpp"

Shell* sh;
AX12* ax12;

struct axreg {
	const char* name;
	uint8_t addr;
	bool twobytes;
};
axreg regs[] = {
	// EEPROM
	{"ID", 0x03, false},
	{"BAUDRATE", 0x04, false},
	{"DELAY", 0x05, false},
	{"CW_LIMIT", 0x06, true},
	{"CCW_LIMIT", 0x08, true},
	{"MAX_TEMP", 0x0B, false},
	{"MIN_VOLT", 0x0C, false},
	{"MAX_VOLT", 0x0D, false},
	{"EE_MAX_TORQUE", 0x0E, true},
	{"RETURN", 0x10, false},
	{"ALARM_LED", 0x11, false},
	{"ALARM_SHUTDOWN", 0x12, false},
	//RAM
	{"TORQUE_ENABLE", 0x18, false},
	{"LED", 0x19, false},
	{"GOAL_POS", 0x1E, true},
	{"GOAL_SPEED", 0x20, true},
	{"MAX_TORQUE", 0x22, true},
	{"POS", 0x24, true},
	{"SPEED", 0x26, true},
	{"LOAD", 0x28, true},
	{"VOLT", 0x2A, false},
	{"TEMP", 0x2B, false},
	{"MOVING", 0x2E, false}
};

void dump(int argc, char** argv) {
	uint8_t val8;
	uint16_t val16;

	(void) argc; (void) argv;

	if(ax12->ping() == 0) { // check AX12 is available
		for(int i = 0; i < 23; i++) {
			int j = 0;

			while(regs[i].name[j] != 0) Serial.write(regs[i].name[j++]); // print the name of the register
			while(j++ < 15)  Serial.write(' '); // print spaces for alignement
			Serial.print(": ");

			if(regs[i].twobytes) {
				ax12->read16(regs[i].addr, &val16);
				Serial.print((int) val16);
			} else {
				ax12->read8(regs[i].addr, &val8);
				Serial.print((int) val8);
			}

			Serial.write('\n');
		}
	}
}

void scan(int argc, char** argv) {
	(void) argc; (void) argv;
	uint8_t savedID = ax12->id;

	Serial.print("Reachable IDs: ");
	ax12->verbose = false;
	for(int i = 0; i < 254; i++) {
		ax12->id = i;
		if(ax12->ping() == 0) {
			Serial.print((int) ax12->id);
			Serial.write(' ');
		}
	}
	ax12->verbose = true;
	Serial.write('\n');

	ax12->id = savedID;
}

void setBaudrate(int argc, char** argv) {
	if(argc != 2) {
		Serial.print("Syntax: baud <serial baudrate> : set AX12 serial port speed\n");
	} else {
		Serial1.flush();
		Serial1.begin(str2int(argv[1]));
	}
}

void setid(int argc, char** argv) {
	if(argc != 2) {
		Serial.print("Syntax: id <new ID> : set current AX12 ID\n");
	} else {
		ax12->id = str2int(argv[1]);
	}
}

void accessReg(int argc, char** argv) {
	uint8_t i, j, val8;
	uint16_t val16;

	(void) argc; (void) argv;

	// find the index of the register
	for(i = 0; i < 23; i++) {
		j = 0;
		// check every character matches until the end
		while(regs[i].name[j] == argv[0][j] && argv[0][j] != 0)
			j++;
		// if the end was reached, we found the match
		if(argv[0][j] == 0 && regs[i].name[j] == 0)
		 	break;
	}

	if(argc == 1) { // only the command -> read
		if(regs[i].twobytes) {
			ax12->read16(regs[i].addr, &val16);
			Serial.print((int) val16);
		} else {
			ax12->read8(regs[i].addr, &val8);
			Serial.print((int) val8);
		}
		Serial.write('\n');
	} else if(argc == 2) { // command + value -> write
		if(regs[i].twobytes) {
			ax12->write16(regs[i].addr, (uint16_t) str2int(argv[1]));
		} else {
			ax12->write8(regs[i].addr, (uint8_t) str2int(argv[1]));
		}
	} else {
		Serial.print("Syntax : COMMAND [value to write] (if value to write omitted, register is read)\n");
	}
}

void errorTest(int argc, char** argv) {
	uint8_t value;
	(void) argc; (void) argv;

	Serial.print("Running 400 ID reads ...");
	Serial.flush();

	ax12->errorCounter = 0;
	ax12->verbose = false;
	for (int i = 0; i < 400; i++) {
		ax12->read8(0x03, &value);
	}
	ax12->verbose = true;

	Serial.print(" got ");
	Serial.print(ax12->errorCounter);
	Serial.print(" errors\n");
}

const command_t comms[] = {
	{"dump", dump},
	{"scan", scan},
	{"id", setid},
	{"baud", setBaudrate},
	{"errortest", errorTest},
	{"ID", accessReg},
	{"BAUDRATE", accessReg},
	{"DELAY", accessReg},
	{"CW_LIMIT", accessReg},
	{"CCW_LIMIT", accessReg},
	{"MAX_TEMP", accessReg},
	{"MIN_VOLT", accessReg},
	{"MAX_VOLT", accessReg},
	{"EE_MAX_TORQUE", accessReg},
	{"RETURN", accessReg},
	{"ALARM_LED", accessReg},
	{"ALARM_SHUTDOWN", accessReg},
	{"TORQUE_ENABLE", accessReg},
	{"LED", accessReg},
	{"GOAL_POS", accessReg},
	{"GOAL_SPEED", accessReg},
	{"MAX_TORQUE", accessReg},
	{"POS", accessReg},
	{"SPEED", accessReg},
	{"LOAD", accessReg},
	{"VOLT", accessReg},
	{"TEMP", accessReg},
	{"MOVING", accessReg},
	{NULL, NULL}
};

void printHelp() {
	Serial.print("available commands:\n");
	Serial.print("  id <ID> : set working ID, subsequent commands will target AX12 with given ID\n");
	Serial.print("  baud <baudrate> : set AX12 serial port speed\n");
	Serial.print("  scan : print all AX12 IDs responding to a ping\n");
	Serial.print("  dump : show all AX12 registers value\n");
	Serial.print("  errortest : test link reliability, do 200 reads and print error count\n");
	Serial.print("  <name of a register> : return the register current value\n");
	Serial.print("  <name of a register> <value>: write a value to an AX12 register\n");
	Serial.print("      (available names are listed with a dump command)\n\n");
}

void shellUnknownCommand() {
	Serial.print("Unknown command\n");
	printHelp();
}

// print shell invite
void onShellInvite() {
	Serial.print((int) ax12->id);
	Serial.print(" > ");
}

// the setup function runs once when you press reset or power the board
void setup() {
	// on the standalone board, the electronics uses a nearby IO as low current 3.3V power supply
	// pinMode(17, OUTPUT);
	// digitalWrite(17, HIGH);

	// setup AX12 serial port
	AX12::init(&Serial1, 115200);
	ax12 = new AX12(0xFE);

	sh = new Shell(115200, comms, onShellInvite);
	sh->onUnknownCommand(shellUnknownCommand);

	Serial.print("\n                    ===  AX12 interactive console ===\n");
	printHelp();
	onShellInvite();
}

// the loop function runs over and over again forever
void loop() {
	sh->update();
	AX12::update();
	delay(20);
}
