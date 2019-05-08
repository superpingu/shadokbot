#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define MAP_SIZE 720
#define MAX_AGE 100

typedef struct Map_Data_s {
    uint32_t distance;
    uint32_t age;
} Map_Data_t;

class Map {
public:
    Map();
    int setDataPoint(uint32_t index, uint32_t distance);
    uint32_t getDistance(uint32_t index);
    void incrementAge();
    void print();
private:
    Map_Data_t data[MAP_SIZE];
};

#endif // MAP_H
