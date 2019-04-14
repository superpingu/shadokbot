#include "circ_buffer.hpp"
#include <stdlib.h>
#include <string.h>

CircBuffer::CircBuffer(int capacity) {
    buffer = (uint8_t*)malloc(capacity);
    memset(buffer, 0, capacity);
    read_ptr = buffer;
    write_ptr = buffer;
    last_addr = buffer + capacity - 1;
    wrap = false;
}

CircBuffer::~CircBuffer() {
    free(buffer);
}

uint8_t CircBuffer::read(uint8_t* ptr) {
    uint8_t ret = 1;
    if (ptr != NULL) {
        if (isDataAvailable()) {
            *ptr = *read_ptr;
            read_ptr = next_read_addr(read_ptr);
            ret = 0;
        }
    }

    return ret;
}

uint8_t CircBuffer::write(uint8_t data) {
    *write_ptr = data;
    write_ptr = next_write_addr(write_ptr);
    return 0;
}

bool CircBuffer::isDataAvailable() {
    if (wrap) {
        if (read_ptr >= write_ptr)
            return true;
    } else {
        if (read_ptr < write_ptr)
            return true;
    }

    return false;
}

uint8_t* CircBuffer::next_write_addr(uint8_t* ptr) {
    if (ptr == last_addr) {
        wrap = true;
        return buffer;
    } else {
        return ptr + 1;
    }
}

uint8_t* CircBuffer::next_read_addr(uint8_t* ptr) {
    if (ptr == last_addr) {
        wrap = false;
        return buffer;
    } else {
        return ptr + 1;
    }
}
