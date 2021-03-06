#include "Shell.hpp"
#include <stdlib.h>
#include <Arduino.h>

int str2int(char* str) {
	uint8_t i = 0;
	int result = 0;
	if(str[0] == '-' && str[1] >= '0' && str[1] <= '9') {
		result = -(str[1]-'0');
		i = 2;
	}
	while(str[i] != 0) {
		if(str[i] >= '0' && str[i] <= '9')
			result = result*10 + str[i] - '0';
		else
			return 0;
		i++;
	}
	return result;
}

void Shell::processCommand() {
	uint8_t i = 0, j;
	while(commands[i].name != NULL) {
		j = 0;
		while(commands[i].name[j] == buffer[j]) {
			if(buffer[j] == 0) {
				commands[i].callback(argc, argv);
				return;
			}
			j++;
		}
		i++;
	}

	if(onUnknownComm == NULL)
		Serial.print("Unknown command\n");
	else
		onUnknownComm();
}

Shell::Shell(uint32_t baudrate, const command_t* _commands, void (*inviteCallback)(void)) {
	index = 0;
	commands = _commands;
	onInviteCallback = inviteCallback;

	onUnknownComm = NULL;
	Serial.begin(baudrate);

	if(onInviteCallback == NULL)
		Serial.print(" > ");
	else
		onInviteCallback();
}

void Shell::update() {
	char newchar;

	while(Serial.available() > 0) {
		newchar = Serial.read();
		Serial.print(newchar);

		if(newchar == '\n') {
			buffer[index] = 0;
			processCommand();
			index = 0;
			argc = 0;
			
			if(onInviteCallback == NULL)
				Serial.print(" > ");
			else
				onInviteCallback();
		} else if(newchar == ' ') {
			buffer[index++] = 0;
		} else if(newchar != '\r') { // ignore \r but add all other character to the buffer
			if(index == 0 || buffer[index-1] == 0) {
				argv[argc] = &buffer[index];
				argc++;
			}
			buffer[index++] = newchar;
		}
	}
}

void Shell::onUnknownCommand(void (*callback)(void)) {
	onUnknownComm = callback;
}
