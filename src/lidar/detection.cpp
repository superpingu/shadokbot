#include "detection.hpp"
#include "Arduino.h"
#include "utils/trigo.hpp"
#include "board.h"
#include "motion/AbsoluteMotion.hpp"
#include "utils/table.hpp"

#define DETECTION_THRESHOLD 400 // in mm
#define COHERENCY_WINDOW_SIZE 3
#define COHERENCY_THRESHOLD 10
#define OBSTACLE_MIN_SIZE 4

#define DETECTION_MARGIN 50 // in mm

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

void Detection::update() {
    static int count = 0;
    int motionDirection;

    robotPosition.x = motion->getX();
    robotPosition.y = motion->getY();
    motionDirection = motion->getMotionDirection();
    map.setRobotAngle(motion->getHeading());

    // Convert motionDirection to half-degree for use with Map
    // and set it inside [0;MAP_SIZE] range
    motionDirection *= 2;
    while (motionDirection < 0)
        motionDirection += MAP_SIZE;

    motionDirection = motionDirection % MAP_SIZE;

    lidar.update();
    map.incrementAge();

    count++;
    if (count == 100) {
        //map.print();
        count = 0;
    }

    bool detected = false;

    for (int i = motionDirection - MAP_SIZE/8; i < motionDirection + MAP_SIZE/8; i++) {
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
        emergencyStop();
	} else {
        emergencyResume();
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

    obstacle.x += distance * mcos((angle + MAP_SIZE / 4) * 0.00873); // ((angle + MAP_SIZE / 4)/2 * PI/180
    obstacle.y += distance * msin((angle + MAP_SIZE / 4) * 0.00873);

    if ((obstacle.x > DETECTION_MARGIN)
        && (obstacle.x < TABLE_MAX_X - DETECTION_MARGIN)
        && (obstacle.y > DETECTION_MARGIN)
        && (obstacle.y < TABLE_MAX_Y - DETECTION_MARGIN))
    {
        return true;
    } else {
        return false;
    }
}

void Detection::emergencyStop() {
    digitalWrite(RED_LED, HIGH);
    motion->emergencyStop();
}

void Detection::emergencyResume() {
    digitalWrite(RED_LED, LOW);
    motion->emergencyResume();
}
