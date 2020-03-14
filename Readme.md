# Available make targets
  ```
  make // to build the Arduino executable
  make host // to build the simulator
  make lidar // to build the host binary to receive data from lidar
```

# Simulator
To run the simulator, execute
```
./build/shadokbot [-i <inputFile>] [-o <outputFile>]
```
For the graphical display, the axis origin is located in the upper left corner.
The X axes is oriented to the right and the Y axis is oriented to the bottom.

A right click somewhere on the table will trigger a move of the robot toward this point (if the robot is not already moving).
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
