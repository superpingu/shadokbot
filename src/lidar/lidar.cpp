#include "lidar.hpp"

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
        content_length = 2 * data + 4; // 2 bytes per sample + 4 bytes for angles
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
