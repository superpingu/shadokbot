#include <lidar/lidar.hpp>
#include <stdio.h>

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    Lidar lidar;
    if (lidar.getStage() != IDLE) {
        printf("Invalid initial stage\n");
        return 1;
    }
    lidar.pushSampleData(0xAA);
    if (lidar.getStage() != HEADER_START) {
        printf("Invalid stage (%d/%d)\n", lidar.getStage(), HEADER_START);
        return 1;
    }

    printf("Lidar parse state machine OK\n");

    return 0;
}
