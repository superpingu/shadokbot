#include "lidar.hpp"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

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
    uint16_t start_angle, finish_angle;
    int16_t ang_corr, angle, angle_step;

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

    uint16_t distances[sample_quantity];
    for (int i = 0; i < sample_quantity; i++) {
        buffer.read(&data);
        buffer.read(&data2);
        distances[i] = (data | (data2 << 8)) / 4;
    }

    // Start angle
    start_angle = raw_start_angle >> 7;
    if (distances[0] != 0) {
        ang_corr = (8 * (155.3-distances[0])/distances[0]);
        start_angle += ang_corr;
    }

    // Finish angle
    finish_angle = raw_finish_angle >> 7;
    if (distances[sample_quantity - 1] != 0) {
        ang_corr = (8 * (155.3-distances[sample_quantity - 1])/distances[sample_quantity - 1]);
        finish_angle += ang_corr;
    }

    angle_step = ABS((finish_angle - start_angle) / (sample_quantity - 1));

    for (int i = 0; i < sample_quantity; i++) {
        if (distances[i] == 0) {
            ang_corr = 0;
        } else {
            ang_corr = (8 * (155.3-distances[i])/distances[i]);
        }
        angle = start_angle + i * angle_step + ang_corr;
        (void)angle;
#if DEBUG
    printf("Angle %d dis: %d", angle, distances[i]);
#endif
    }
}


#if DEBUG
Parsing_Stage_t Lidar::getStage() {
    return stage;
}
#endif
