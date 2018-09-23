# WARNING : platform specific ! use "arduino" for linux,
# "/Application/Arduino.app/content/MacOS/Arduino" for MacOS (and don't use windows)
CC = /Applications/Arduino.app/Contents/MacOS/Arduino
# Arduino's virtual serial port name
PORT = /dev/tty.usbmodem*
# target board
TBOARD = arduino:avr:mega:cpu=atmega2560
# main sketch
SKETCH = shadokbot.ino

verify:
	$(CC) --board $(TBOARD) --verify $(SKETCH)

upload:
	$(CC) --board $(TBOARD) --port $(PORT) --upload $(SKETCH)

test:
	g++ -std=c++11 -o test/test test/test.cpp -lboost_iostreams -lboost_system -lboost_filesystem
	test/test

.PHONY: test
