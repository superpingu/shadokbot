#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include "circ_buffer.hpp"

#if DEBUG
#define LOG(str, ...) printf(str "\n", __VA_ARGS__)
#else
#define LOG(str,...) (void)0
#endif

typedef struct Raw_Data_s {
    uint16_t raw_start_angle;
    uint16_t raw_finish_angle;
    uint8_t sample_quantity;
    uint32_t distances[255];
} Raw_Data_t;

typedef enum Parsing_Stage_s {
    IDLE,
    HEADER_START,
    HEADER_END,
    CONTENT_TYPE,
    LENGTH
} Parsing_Stage_t;

class Lidar {
public:
    Lidar();
    ~Lidar();
    void pushSampleData(uint8_t data);
    uint32_t* getMap();
#if DEBUG
    Parsing_Stage_t getStage();
    void printMap();
#endif

private:
    CircBuffer buffer;
    int content_length;
    Parsing_Stage_t stage;
    Raw_Data_t raw_data;
    uint32_t map[360];

    void parseFrame();
    void updateMap();
    int32_t computeAngCorr(uint32_t distance);
    uint16_t convertAngle(int32_t angle);

};

#endif // LIDAR_H
