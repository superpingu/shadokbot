#ifndef DETECTION_HPP
#define DETECTION_HPP

#include "map.h"
#include "lidar.hpp"

typedef struct Position_s {
    int32_t x;
    int32_t y;
} Position_t;

class Detection {
public:
    Detection();
    void init();
    void update();
	Map map;
	Lidar lidar;
private:

    Position_t robotPosition;

    bool isNoise(int32_t angle);
    bool isOnTable(int32_t angle, uint32_t distance);
};

#endif // DETECTION_HPP
