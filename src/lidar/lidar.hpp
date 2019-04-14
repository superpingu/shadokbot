#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include "circ_buffer.hpp"

typedef enum Parsing_Stage_s {
    IDLE,
    HEADER_START,
    HEADER_END,
    CT,
    LENGTH
} Parsing_Stage_t;

class Lidar {
public:
    Lidar();
    ~Lidar();
    void pushSampleData(uint8_t data);

private:
    CircBuffer buffer;
    int content_length;
    Parsing_Stage_t stage;

    void parseFrame();

};

#endif // LIDAR_H
