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

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fd = fopen(argv[1], "r");
    char c;
    int tmp, val;
    bool second = false;
    while ((c = getc(fd)) != EOF) {
        if ((c >= '0') && (c <= '9')) {
            tmp = c - '0';
        } else if ((c >= 'a') && (c <= 'f')) {
            tmp = c - 'a' + 10;
        } else {
            continue;
        }

        if (second) {
            second = false;
            val += tmp;
            lidar.pushSampleData(val);
        } else {
            second = true;
            val = tmp << 4;
        }
    }

    fclose(fd);
    return 0;
}
