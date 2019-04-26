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
endif

# end platform dependent configuration

BOARD_TAG    = arduino_due_x
ARCHITECTURE = sam
# prevent verbose logging from Arduino-Makefile
ARDUINO_QUIET = 1
# arduino monitor serial port baudrate
MONITOR_BAUDRATE = 115200

CFLAGS_STD = -std=gnu11
CXXFLAGS_STD = -std=gnu++11 -Wall -Wextra -I$(SRCDIR)

# main file
LOCAL_INO_SRCS = $(SRCDIR)/main.ino
# project sources
LOCAL_CPP_SRCS = $(SRCDIR)/shell/commands.cpp $(SRCDIR)/shell/Shell.cpp \
	$(SRCDIR)/ax12/AXcomms.cpp $(SRCDIR)/ax12/AX12.cpp $(SRCDIR)/hal/Timer.cpp \
	$(SRCDIR)/motion/Motor.cpp $(SRCDIR)/motion/Motion.cpp

include $(ARDMK_DIR)/Sam.mk

AX12:
	make LOCAL_INO_SRCS=AX12console/AX12console.ino

AX12upload:
	make upload LOCAL_INO_SRCS=AX12console/AX12console.ino
