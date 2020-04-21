# Shadokbot

Shadoks Robotique robot embedded software and simulator.
The embedded software targets an Arduino Due board, containing a Atmel SAM5 processor (ARM Cortex M3 core).


## Installation

The build structure supports Linux and MacOS.

To build the embedded software, you need:
* the Arduino IDE with Arduino Due support. See [the Arduino documentation](https://www.arduino.cc/en/Guide/ArduinoDue#toc2)
to setup the Arduino IDE.
* [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) to compile and upload
from command line using make:

To install Arduino-Makefile on MacOS, use `brew install --HEAD arduino-mk`.
The --HEAD option is mandatory to have the SAM processor support.

To instal Arduino-Makefile on Linux ...

You might have to update the platform dependent configuration at the beginning of the Makefile.


#### Compile and upload the embedded software

To compile, just run `make` at the root of the repository.

To upload, make sure the Arduino is connected using its programming port and its serial
port is not used by another app.
Then run `make upload` at the root of the repository.

#### Simulator

To compile the simulator, you need the embedded software dependencies and SFML library.

To install SFML on MacOS, just run `brew install SFML`.

To install SFLM on Linux ...

Then run `make host` to compile the simulator.

#### LIDAR App

A small app is provided to visualize the LIDAR data. To build it, run `make lidar`.


## Simulator

To run the simulator, execute
```
./build/shadokbot [-i <inputFile>] [-o <outputFile>]
```
For the graphical display, the axis origin is located in the upper left corner.
The X axes is oriented to the right and the Y axis is oriented to the bottom.

A left click somewhere on the table will trigger a move of the robot toward this point (if the robot is not already moving).
When using an output file, don't forger to close it (by pressing "q") before closing the program if you want to save the path.

Next feature: Obstacles
If provided, the *obstacle_file* must have the following syntax, one line per obstacle:
```
x y length width angle
```

List of replaced files compared to actual embedded code:
* actions/sequence.cpp -> simu_sequence.cpp (implement both simu_sequence.hpp and actions/sequence.hpp)
* utils/Timer.cpp -> mockup/dummy_timer.cpp
* ax12/* -> mockup/dummy_ax12.cpp
* display/SevenSegDisplay.cpp -> mockup/dummy_display.cpp
* imu/IMU.cpp -> mockup/dummy_imu.cpp
* motion/Motor.cpp -> mockup/dummy_motor.cpp
* motion/Motion.cpp -> mockup/dummy_motion.cpp
* lidar/lidar.cpp -> mockup/dummy_lidar.cpp
