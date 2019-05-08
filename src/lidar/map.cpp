#include "map.h"
#include <string.h>
#include "Arduino.h"

Map::Map() {
    memset(data, 0, sizeof(data));
}

int Map::setDataPoint(uint32_t index, uint32_t distance) {
    if (index >= MAP_SIZE) {
        return -1;
    }

    data[index].distance = distance;
    data[index].age = 0;

    return 0;
}

uint32_t Map::getDistance(uint32_t index) {
    if ((index >= MAP_SIZE) || (data[index].age > MAX_AGE)) {
        return 0;
    }

    return data[index].distance;
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
