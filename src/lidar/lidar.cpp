#include "lidar.hpp"
#if DEBUG
#include <stdio.h>
#endif

#define FIXED_POINT_MULTIPLIER 1000
#define ANG_CORR_CONST (155.3 * FIXED_POINT_MULTIPLIER)

Lidar::Lidar() :
    buffer(200) {
    stage = IDLE;
}

Lidar::~Lidar() {

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
            stage = CT;
        else
            stage = IDLE;
        break;
    case CT:
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
    uint8_t data, ret, sample_quantity, data2;
    uint16_t raw_start_angle, raw_finish_angle;
    uint32_t start_angle, finish_angle;
    int32_t angle, angle_step;

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
    buffer.read(&sample_quantity);

    // Starting angle
    buffer.read(&data);
    buffer.read(&data2);
    raw_start_angle = data | (data2 << 8);

    // Finishing angle
    buffer.read(&data);
    buffer.read(&data2);
    raw_finish_angle = data | (data2 << 8);

    // Check code
    buffer.read(&data);
    buffer.read(&data);

    uint32_t distances[sample_quantity];
    for (int i = 0; i < sample_quantity; i++) {
        buffer.read(&data);
        buffer.read(&data2);
        distances[i] = (data | (data2 << 8)) / 4;
    }

    // Fixed point with 3 decimals
    for (int i = 0; i < sample_quantity; i++) {
        distances[i] *= FIXED_POINT_MULTIPLIER;
    }

    // Start angle
    start_angle = raw_start_angle >> 7;
    start_angle *= FIXED_POINT_MULTIPLIER;
    start_angle += computeAngCorr(distances[0]);

    // Finish angle
    finish_angle = raw_finish_angle >> 7;
    finish_angle *= FIXED_POINT_MULTIPLIER;
    finish_angle += computeAngCorr(distances[sample_quantity - 1]);

    if (sample_quantity > 1)
        if (finish_angle >= start_angle)
            angle_step = (finish_angle - start_angle) / (sample_quantity - 1);
        else
            angle_step = ((360 * FIXED_POINT_MULTIPLIER) + finish_angle - start_angle) / (sample_quantity - 1);
    else
        angle_step = 0;

    LOG("Start %d Finish %d Quantity %d Step %d", start_angle, finish_angle, sample_quantity, angle_step);

    for (int i = 0; i < sample_quantity; i++) {
        angle = start_angle + i * angle_step + computeAngCorr(distances[i]);
        (void)angle;
        LOG("Angle %d dis: %d", angle, distances[i]);
    }
}

int32_t Lidar::computeAngCorr(uint32_t distance) {
    int32_t ang_corr;
    if (distance != 0)
        ang_corr = (8047 * (ANG_CORR_CONST - distance)) / distance; // 8047 = 1000 * (21.8 * 180) / (155.3 * PI)
    else
        ang_corr = 0;

    return ang_corr;
}

#if DEBUG
Parsing_Stage_t Lidar::getStage() {
    return stage;
}
#endif
