#include "detection.hpp"
#include "Arduino.h"
#include <math.h>
#include "board.h"
#include "motion/AbsoluteMotion.hpp"

#define DETECTION_THRESHOLD 300 // in mm
#define COHERENCY_WINDOW_SIZE 3
#define COHERENCY_THRESHOLD 10
#define OBSTACLE_MIN_SIZE 4

#define TABLE_MAX_X 3000 // in mm
#define TABLE_MAX_Y 2000 // in mm
#define TABLE_MARGIN 50 // in mm

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

Detection::Detection() :
    map(),
    lidar(&map)
{
    robotPosition.x = 0;
    robotPosition.y = 0;
}

void Detection::init() {
    lidar.init(LIDAR_SERIALPORT, 128000);
    lidar.startScan();
}

void Detection::update(int32_t newRobotX, int32_t newRobotY, int32_t movementOrientation) {
    static int count = 0;
    lidar.update();
    map.incrementAge();

    count++;
    if (count == 100) {
        //map.print();
        count = 0;
    }

    robotPosition.x = newRobotX;
    robotPosition.y = newRobotY;
    bool detected = false;
    for (int i = movementOrientation - MAP_SIZE/4; i < movementOrientation + MAP_SIZE/4; i++) {
        uint32_t curDistance = map.getDistance(i);
        if ((curDistance != 0) && (curDistance <= DETECTION_THRESHOLD)) {
            if (isNoise(i)) {
                i += COHERENCY_WINDOW_SIZE;
            } else if (isOnTable(i, curDistance)) {
                detected = true;
                break;
            }
        }
    }

    if (detected) {
		digitalWrite(RED_LED, HIGH);
		motion->emergencyStop();
	} else {
        digitalWrite(RED_LED, LOW);
		motion->emergencyResume();
	}
}

bool Detection::isNoise(int32_t angle) {
    int closePointsCounter = 0;
    int32_t baseDistance = map.getDistance(angle);
    for (int i = angle - COHERENCY_WINDOW_SIZE; i <= angle + COHERENCY_WINDOW_SIZE; i++) {
        if (ABS(baseDistance - (int32_t)map.getDistance(i)) < COHERENCY_THRESHOLD) {
            closePointsCounter++;
            if (closePointsCounter >= OBSTACLE_MIN_SIZE)
                return false;
        }
    }

    return true;
}

bool Detection::isOnTable(int32_t angle, uint32_t distance) {
    Position_t obstacle = {robotPosition.x, robotPosition.y};

    obstacle.x += distance * cos((angle + MAP_SIZE / 4) * 0.00873); // ((angle + MAP_SIZE / 4)/2 * PI/180
    obstacle.y += distance * sin((angle + MAP_SIZE / 4) * 0.00873);

    if ((obstacle.x > TABLE_MARGIN)
        && (obstacle.x < TABLE_MAX_X - TABLE_MARGIN)
        && (obstacle.y > 400 + TABLE_MARGIN)
        && (obstacle.y < TABLE_MAX_Y - TABLE_MARGIN))
    {
        return true; // Main "free" space
    } else if ((obstacle.y > TABLE_MARGIN)
               && (obstacle.y < 400 + TABLE_MARGIN)
               && (((obstacle.x > TABLE_MARGIN) && (obstacle.x < 450))
                  || ((obstacle.x > 2550) && (obstacle.x < TABLE_MAX_X - TABLE_MARGIN)))) {
        return true; // Base of the slopes
    } else {
        return false;
    }
}
