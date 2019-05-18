#ifndef LIDAR_H
#define LIDAR_H

#include <stdint.h>
#include "circ_buffer.hpp"
#if DEBUG
#include "../test/arduino_mockup.h"
#else
#include <Arduino.h>
#include "utils/Timer.hpp"
#include "map.h"
#endif

#if DEBUG
#define LOG(str, ...) printf(str "\n", __VA_ARGS__)
#else
#define LOG(str,...) (void)0
#endif

#define RESPONSE_START_SIGN_BYTE1 0xA5
#define RESPONSE_START_SIGN_BYTE2 0x5A

#define COMMAND_START_BYTE 0xA5
#define COMMAND_CMD_START_SCAN 0x60

#define OFFSET_RESP_START_SIGN_1 0
#define OFFSET_RESP_START_SIGN_2 1

#define ANGLE_MAX 720

#define MIN_DIST (150 * FIXED_POINT_MULTIPLIER)// in mm
#define MAX_DIST (5000 * FIXED_POINT_MULTIPLIER) // in mm

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
    Lidar(Map *pMap);
    ~Lidar();
    void pushSampleData(uint8_t data);
    void init(HardwareSerial& serial, int baudrate);
    void startScan();
    void readCb();
    void update();
#if DEBUG
    Parsing_Stage_t getStage();
    void printMap();
#endif

private:
    CircBuffer buffer;
    int content_length;
    Parsing_Stage_t stage;
    Raw_Data_t raw_data;
    Map *map;
    HardwareSerial *serial;
    Timer readTimer;
    uint32_t frameToParse;

    void updateMap();
    int32_t computeAngCorr(uint32_t distance);
    uint16_t convertAngle(int32_t angle);

    void parseFrame();
    bool needParsing();
};

#endif // LIDAR_H
