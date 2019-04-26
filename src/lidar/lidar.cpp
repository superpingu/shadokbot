#include "lidar.hpp"
#include <string.h>
#if DEBUG
#include <stdio.h>
#endif

#define FIXED_POINT_MULTIPLIER 1000
#define ANG_CORR_CONST (155.3 * FIXED_POINT_MULTIPLIER)

Lidar::Lidar() :
    buffer(200) {
    stage = IDLE;
    memset(&map, 0, sizeof(map));
}

Lidar::~Lidar() {

}

void Lidar::init(HardwareSerial& serial, int baudrate) {
    this->serial = &serial;
    this->serial->begin(baudrate);
}

void Lidar::startScan() {
    int byte, offset = 0, length = 0;
    serial->write(COMMAND_START_BYTE);
    serial->write(COMMAND_CMD_START_SCAN);
    while (1) {
        byte = serial->read();
        if (byte < 0) continue;
        switch (offset) {
        case OFFSET_RESP_START_SIGN_1:
            if (byte != RESPONSE_START_SIGN_BYTE1)
                continue;
            break;
        case OFFSET_RESP_START_SIGN_2:
            if (byte != RESPONSE_START_SIGN_BYTE2)
                continue;
            break;
        default:
            break;
        }
        offset++;
        if (offset == 7 + length)
            break;

    }
}

void Lidar::pushSampleData(uint8_t data) {
    buffer.write(data);
    switch (stage) {
    case IDLE:
        if (data == 0xAA)
            stage = HEADER_START;
        break;
    case HEADER_START:
        if (data == 0x55)
            stage = HEADER_END;
        else
            stage = IDLE;
        break;
    case HEADER_END:
        if (data == 0x00 || data == 0x01)
            stage = CONTENT_TYPE;
        else
            stage = IDLE;
        break;
    case CONTENT_TYPE:
        content_length = 2 * data + 6; // 2 bytes per sample + 4 bytes for angles + 2 bytes for check code
        stage = LENGTH;
        break;
    case LENGTH:
        content_length--;
        if (content_length == 0) {
            stage = IDLE;
            parseFrame();
        }
    }
}

void Lidar::parseFrame() {
    uint8_t data, ret, data2;

    memset(&raw_data, 0, sizeof(raw_data));
    ret = buffer.read(&data);
    while ((ret == 0) && (data != 0xAA)) {
        ret = buffer.read(&data);
    }
    buffer.read(&data);
    if (data != 0x55) {
        parseFrame();
        return;
    }
    buffer.read(&data);
    if (data != 0) { // Zero packet frame
        return;
    }
    buffer.read(&raw_data.sample_quantity);

    // Starting angle
    buffer.read(&data);
    buffer.read(&data2);
    raw_data.raw_start_angle = data | (data2 << 8);

    // Finishing angle
    buffer.read(&data);
    buffer.read(&data2);
    raw_data.raw_finish_angle = data | (data2 << 8);

    // Check code
    buffer.read(&data);
    buffer.read(&data);

    for (int i = 0; i < raw_data.sample_quantity; i++) {
        buffer.read(&data);
        buffer.read(&data2);
        raw_data.distances[i] = (data | (data2 << 8)) / 4;
    }

    updateMap();
}

void Lidar::updateMap() {
    uint32_t start_angle, finish_angle;
    int32_t angle, angle_step;

    // Fixed point with 3 decimals
    for (int i = 0; i < raw_data.sample_quantity; i++) {
        raw_data.distances[i] *= FIXED_POINT_MULTIPLIER;
    }

    // Start angle
    start_angle = raw_data.raw_start_angle >> 7;
    start_angle *= FIXED_POINT_MULTIPLIER;
    start_angle += computeAngCorr(raw_data.distances[0]);

    // Finish angle
    finish_angle = raw_data.raw_finish_angle >> 7;
    finish_angle *= FIXED_POINT_MULTIPLIER;
    finish_angle += computeAngCorr(raw_data.distances[raw_data.sample_quantity - 1]);

    if (raw_data.sample_quantity > 1)
        if (finish_angle >= start_angle)
            angle_step = (finish_angle - start_angle) / (raw_data.sample_quantity - 1);
        else
            angle_step = ((360 * FIXED_POINT_MULTIPLIER) + finish_angle - start_angle) / (raw_data.sample_quantity - 1);
    else
        angle_step = 0;

    LOG("Start %d Finish %d Quantity %d Step %d", start_angle, finish_angle, raw_data.sample_quantity, angle_step);

    for (int i = 0; i < raw_data.sample_quantity; i++) {
        angle = start_angle + i * angle_step + computeAngCorr(raw_data.distances[i]);
        map[convertAngle(angle)] = raw_data.distances[i] / FIXED_POINT_MULTIPLIER;
    }

#if DEBUG
    printMap();
#endif
}

int32_t Lidar::computeAngCorr(uint32_t distance) {
    int32_t ang_corr;
    if (distance != 0)
        ang_corr = (8047 * (ANG_CORR_CONST - distance)) / distance; // 8047 = 1000 * (21.8 * 180) / (155.3 * PI)
    else
        ang_corr = 0;

    return ang_corr;
}

uint16_t Lidar::convertAngle(int32_t angle) {
    angle /= FIXED_POINT_MULTIPLIER;
    while (angle >= 360) {
        angle -= 360;
    }
    while (angle < 0) {
        angle += 360;
    }

    return angle;
}

uint32_t* Lidar::getMap() {
    return map;
}

#if DEBUG
Parsing_Stage_t Lidar::getStage() {
    return stage;
}

void Lidar::printMap() {
    for (int i = 0; i < 360; i++) {
        LOG("%i %d", i, map[i]);
    }
}
#endif
