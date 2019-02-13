# Warning : this Makefile uses Arduino-makefile for compilation and upload
# See https://github.com/sudar/Arduino-Makefile/ for install
# to see the available targets, use 'make help'
PROJECT_DIR = $(abspath .)
OBJDIR = $(PROJECT_DIR)/build
SRCDIR = $(PROJECT_DIR)/src

# The configuration below is platform dependent
ARDMK_DIR            = /usr/local/Cellar/arduino-mk/HEAD-4452f77
ARDUINO_DIR          = /Applications/Arduino.app/Contents/Java
ARDUINO_PACKAGE_DIR := $(HOME)/Library/Arduino15/packages
MONITOR_PORT = /dev/tty.usbmodem* # Arduino serial port
# end platform dependent configuration

BOARD_TAG    = arduino_due_x
ARCHITECTURE = sam
# prevent verbose logging from Arduino-Makefile
ARDUINO_QUIET = 1
# arduino monitor serial port baudrate
MONITOR_BAUDRATE = 115200

CFLAGS_STD = -std=gnu11
CXXFLAGS_STD = -std=gnu++11 -Wall -Wextra

# main file
LOCAL_INO_SRCS = $(SRCDIR)/blink.ino
# project sources
LOCAL_CPP_SRCS =

include $(ARDMK_DIR)/Sam.mk
