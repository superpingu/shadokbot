# Warning : this Makefile uses Arduino-makefile for compilation and upload
# See https://github.com/sudar/Arduino-Makefile/ for install
# to see the available targets, use 'make help'
PROJECT_DIR = $(abspath .)
OBJDIR = $(PROJECT_DIR)/build
SRCDIR = $(PROJECT_DIR)/src
SIMUDIR = $(PROJECT_DIR)/simu

# The configuration below is platform dependent
ifeq ($(shell echo $$OSTYPE), darwin19)
	ARDMK_DIR            = /usr/local/Cellar/arduino-mk/HEAD-e870443
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
# compilation flags (force include of SPI.h: on non-case sensitive system it gets confused with SAM HAL file spi.h)
CFLAGS_STD = -std=gnu11
CXXFLAGS_STD = -std=gnu++11 -Wall -Wextra -I$(SRCDIR) -I$(ARDUINO_PLATFORM_LIB_PATH)/Wire/src \
	-include $(ARDUINO_PLATFORM_LIB_PATH)/SPI/src/SPI.h

# main file
LOCAL_INO_SRCS = $(SRCDIR)/main.ino
# project sources (+ arduino SPI, for some reason not compiled by arduino-mk)
LOCAL_CPP_SRCS = $(wildcard $(SRCDIR)/*/*.cpp) $(ARDUINO_PLATFORM_LIB_PATH)/SPI/src/SPI.cpp

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

###################################################### simulator ######################################################

# Simulation source
INO_FILE_AS_CPP=$(patsubst %.ino,%.cpp,$(LOCAL_INO_SRCS))
ACTIONS_CPP_SRCS = $(SRCDIR)/actions/sequence.cpp $(SRCDIR)/actions/robot.cpp
SHELL_CPP_SRCS = $(SRCDIR)/shell/commands.cpp $(SRCDIR)/shell/Shell.cpp

SIMU_SRC=$(SIMUDIR)/mockup/serial.cpp $(SIMUDIR)/mockup/arduino_time.cpp \
	$(SIMUDIR)/mockup/dummy_ax12.cpp $(SIMUDIR)/mockup/Wire.cpp \
	$(SRCDIR)/utils/trigo.cpp $(SIMUDIR)/mockup/dummy_abs_motion.cpp \
	$(SIMUDIR)/mockup/dummy_motion.cpp $(SIMUDIR)/mockup/dummy_motor.cpp $(SIMUDIR)/mockup/dummy_imu.cpp \
	$(SIMUDIR)/main.cpp $(SIMUDIR)/mockup/dummy_lidar.cpp \
	$(SRCDIR)/lidar/circ_buffer.cpp $(SRCDIR)/lidar/map.cpp $(SRCDIR)/lidar/detection.cpp \
	$(SIMUDIR)/mockup/arduino_pin.cpp $(SIMUDIR)/mockup/dummy_timer.cpp $(wildcard $(SRCDIR)/actions/*.cpp) \
	$(SIMUDIR)/mockup/dummy_display.cpp $(wildcard $(SRCDIR)/shell/*.cpp) $(SIMUDIR)/simu_table.cpp \
	$(SIMUDIR)/simu_robot.cpp $(SIMUDIR)/simu_time.cpp

host: $(LOCAL_INO_SRCS) $(SIMU_SRC)
	mkdir -p build
	g++ -x c++ $(LOCAL_INO_SRCS) $(SIMU_SRC) -g -std=c++11 -DSIMU=1 -I$(SIMUDIR)/mockup -I$(SIMUDIR) -I$(SRCDIR) -lm -lsfml-graphics -lsfml-window -lsfml-system -o $(OBJDIR)/shadokbot
