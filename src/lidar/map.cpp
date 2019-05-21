#include "map.h"
#include <string.h>
#include "Arduino.h"

Map::Map() {
    memset(data, 0, sizeof(data));
    robotAngle = 0;
}

int Map::setDataPoint(uint32_t index, uint32_t distance) {
    if (index >= MAP_SIZE) {
        return -1;
    }

    data[index].distance = distance;
    data[index].age = 0;

    return 0;
}

uint32_t Map::getDistance(int32_t absoluteAngle) {
    uint32_t relativeAngle = getIndexFromAbsoluteAngle(absoluteAngle);
    if (data[relativeAngle].age > MAX_AGE) {
        return 0;
    }

    return data[relativeAngle].distance;
}

void Map::setRobotAngle(uint32_t newRobotAngle) {
    robotAngle = newRobotAngle % MAP_SIZE;
}

void Map::incrementAge() {
    for (int i = 0; i < MAP_SIZE; i++)
        data[i].age++;
}

void Map::print() {
    for (int i = 0; i < MAP_SIZE; i++) {
        Serial.print(i);
        Serial.print(" ");
        Serial.println(getDistance(i));
    }
}

uint32_t Map::getIndexFromAbsoluteAngle(uint32_t angle) {
    int32_t relativeAngle = angle - robotAngle;
    while (relativeAngle < 0)
        relativeAngle += MAP_SIZE;

    relativeAngle = relativeAngle % MAP_SIZE;
    // absolute angles are counterclockwise positive while internal (relative) angles
    // are clockwise positive
    return (MAP_SIZE - relativeAngle);
}
