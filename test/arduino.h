#ifndef ARDUINO_H
#define ARDUINO_H

#define HIGH 1
#define LOW 0
#define OUTPUT 0

#include <pthread.h>

int pins[16]; // contains to current state of the arduino pins
void (*delay_callback)() = NULL; // called when an arduino delay is called

// Arduino functions mockup
void pinMode(int pin, int mode) {}
void delayMicroseconds(int delay) { if(delay_callback != NULL) delay_callback(); }
unsigned long micros() { return 0; }
void digitalWrite(int pin, int value) { if(pin < 16) pins[pin] = value; }

std::string newLine;
void* user_input_thread(void* args) {
	while (true) {
		std::string str;
		std::getline(std::cin, str);
		newLine += str + '\n';
	}
}

class SerialPort {
	pthread_t thread;
public:
	void begin(int baudrate) {
		std::cout << "Shell initiated at " << baudrate << " bauds" << '\n';
		pthread_create(&thread, NULL, user_input_thread, NULL);
	}
	void print(const char* str) {
		std::cout << str << std::flush;
	}
	void print(const char c) {
		std::cout << c;
	}
	int available() {
		return newLine.size();
	}
	char read() {
		char res = newLine.at(0);
		newLine.erase(0, 1);
		return res;
	}
};

SerialPort Serial;

#endif
