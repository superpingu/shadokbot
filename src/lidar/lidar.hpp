#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include "circ_buffer.hpp"

class Lidar {
public:
    Lidar();
    ~Lidar();
    void pushSampleData(uint8_t data);

private:
    CircBuffer buffer;

    void parseFrame();
};

#endif // LIDAR_H
