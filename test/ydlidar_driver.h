#ifndef YDLIDAR_DRIVER_H
#define YDLIDAR_DRIVER_H
#include <vector>
#include <string>

struct LaserConfig {
  void* foo;
};

//! A struct for returning laser readings from the YDLIDAR
struct LaserScan {
	//! Array of ranges
	std::vector<float> ranges;
	//! Array of intensities
	std::vector<float> intensities;
	//! Self reported time stamp in nanoseconds
	uint64_t self_time_stamp;
	//! System time when first range was measured in nanoseconds
	uint64_t system_time_stamp;
	//! Configuration of scan
	LaserConfig config;
};

#endif // YDLIDAR_DRIVER_H
