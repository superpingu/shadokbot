#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include "circ_buffer.hpp"

#if DEBUG
#define LOG(str, ...) printf(str "\n", __VA_ARGS__)
#else
#define LOG(str,...) (void)0
#endif

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
#if DEBUG
    Parsing_Stage_t getStage();
#endif

private:
    CircBuffer buffer;
    int content_length;
    Parsing_Stage_t stage;

    void parseFrame();

};

#endif // LIDAR_H
