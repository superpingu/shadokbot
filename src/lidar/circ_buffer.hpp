#ifndef CIRC_BUFFER_H
#define CIRC_BUFFER_H

#include <stdint.h>

class CircBuffer {
public:
    CircBuffer(int capacity);
    ~CircBuffer();
    uint8_t read(uint8_t* ptr);
    uint8_t write(uint8_t data);
    bool isDataAvailable();

private:
    uint8_t* buffer;
    uint8_t* read_ptr;
    uint8_t* write_ptr;
    const uint8_t* last_addr;

    uint8_t wrap;
    uint8_t* next_write_addr(uint8_t* ptr);
    uint8_t* next_read_addr(uint8_t* ptr);
};

#endif // CIRC_BUFFER_H
