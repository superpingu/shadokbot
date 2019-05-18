// create an interactive shell using Arduino main serial port

#ifndef SHELL_HPP
#define SHELL_HPP

#include <stdint.h>
#include <stdlib.h>

#define SHELL_BUFFER_SIZE 70 // maximum number of characters for a single command
#define MAX_ARG_NUMBER 8 // maximum number of arguments + 1

// type for describing commands : when the user enters <command> <arg1> <arg2> ...
// if the command matches name, callback is called.
// Callback prototype should be like void callback(int argc, char** argv)
// where argc is the number of arguments + 1
//       argv contains pointers to argument strings
struct command_t {
	const char* name;
	void (*callback)(int, char**);
};

// convert a string to an integer, returns 0 in case of error
int str2int(char* str);

class Shell {
private:
	const command_t* commands;
	char buffer[SHELL_BUFFER_SIZE];
	uint8_t index;
	char* argv[MAX_ARG_NUMBER];
	uint8_t argc = 0;
	void (*onUnknownComm)(void);
	void (*onInviteCallback)(void);

	void processCommand();
public:
	// Shell initializes Arduino's Serial at baudrate and will accept the provided commands
	// last command must be followed by the entry {NULL, NULL} to end the list
	Shell(uint32_t baudrate, const command_t* _commands, void (*inviteCallback)(void)=NULL);

	// if different from NULL, callback will be called when an unknown or void command is entered
	// by default just prints "Unknown command"
	void onUnknownCommand(void (*callback)(void));

	// call when a new shell invite should be printed, by default prints ' > '
	void onInvite(void (*callback)(void));

	// call in the main loop to process input from serial port
	void update();
};

#endif
