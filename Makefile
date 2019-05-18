# Warning : this Makefile uses Arduino-makefile for compilation and upload
# See https://github.com/sudar/Arduino-Makefile/ for install
# to see the available targets, use 'make help'
PROJECT_DIR = $(abspath .)
OBJDIR = $(PROJECT_DIR)/build
SRCDIR = $(PROJECT_DIR)/src

# The configuration below is platform dependent
ifeq ($(shell echo $$OSTYPE), darwin17)
	ARDMK_DIR            = /usr/local/Cellar/arduino-mk/HEAD-4452f77
	ARDUINO_DIR          = /Applications/Arduino.app/Contents/Java
	ARDUINO_PACKAGE_DIR := $(HOME)/Library/Arduino15/packages
	MONITOR_PORT         = /dev/tty.usbmodem* # Arduino serial port
else
	ARDMK_DIR            = $(HOME)/Documents/TelecomRobotics/Arduino-Makefile/
	ARDUINO_DIR          = $(HOME)/Documents/Arduino/arduino-1.8.5/java/bin/java
	ARDUINO_PACKAGE_DIR  := $(HOME)/.arduino15/packages
	MONITOR_PORT         = /dev/ttyACM1
endif

# end platform dependent configuration

BOARD_TAG    = arduino_due_x
ARCHITECTURE = sam
# prevent verbose logging from Arduino-Makefile
ARDUINO_QUIET = 1
# arduino monitor serial port baudrate
MONITOR_BAUDRATE = 115200

CFLAGS_STD = -std=gnu11
CXXFLAGS_STD = -std=gnu++11 -Wall -Wextra -I$(SRCDIR) -I$(ARDUINO_PLATFORM_LIB_PATH)/Wire/src

# main file
LOCAL_INO_SRCS = $(SRCDIR)/main.ino
# project sources
LIDAR_CPP_SRCS = $(SRCDIR)/lidar/circ_buffer.cpp $(SRCDIR)/lidar/lidar.cpp \
$(SRCDIR)/lidar/map.cpp $(SRCDIR)/lidar/detection.cpp
IMU_CPP_SRCS = $(SRCDIR)/imu/IMU.cpp $(ARDUINO_PLATFORM_LIB_PATH)/Wire/src/Wire.cpp
MOTION_CPP_SRCS = $(SRCDIR)/motion/Motor.cpp $(SRCDIR)/motion/Motion.cpp $(SRCDIR)/motion/AbsoluteMotion.cpp

LOCAL_CPP_SRCS = $(SRCDIR)/shell/commands.cpp $(SRCDIR)/shell/Shell.cpp \
	$(SRCDIR)/ax12/AXcomms.cpp $(SRCDIR)/ax12/AX12.cpp $(SRCDIR)/utils/Timer.cpp \
	$(SRCDIR)/utils/trigo.cpp $(LIDAR_CPP_SRCS) $(IMU_CPP_SRCS) $(MOTION_CPP_SRCS)

include $(ARDMK_DIR)/Sam.mk

AX12:
	make LOCAL_INO_SRCS=AX12console/AX12console.ino

AX12upload:
	make upload LOCAL_INO_SRCS=AX12console/AX12console.ino

LIDAR_TEST_SRCS = $(LIDAR_CPP_SRCS) test/lidar_test.cpp test/arduino_mockup.cpp
lidar_test: $(LIDAR_TEST_SRCS)
	g++ -DDEBUG=1 $(CXXFLAGS_STD) $(LIDAR_TEST_SRCS) -o $(OBJDIR)/lidar_test

host_lidar: test/lidar_host.c lib/gnuplot_i/src/gnuplot_i.c
	mkdir -p build
	gcc $^ -o $(OBJDIR)/lidar_host -lm
