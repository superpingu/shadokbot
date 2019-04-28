#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include </home/sylvain/Documents/TelecomRobotics/X4/gnuplot_i/src/gnuplot_i.h>
#include <math.h>

#define ANGLE_MAX 720

// Copied from https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

typedef enum {
    UNKNOWN,
    ANGLE,
    DISTANCE
} Status_e;

char *portname = "/dev/ttyACM1";
int main(int argc, char *argv[]) {
    gnuplot_ctrl *h = gnuplot_init();
    double x[ANGLE_MAX];
    double y[ANGLE_MAX];

    memset(x, 0, sizeof(x));
    memset(y, 0, sizeof(y));
    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf ("error %d opening %s: %s", errno, portname, strerror (errno));
        return 1;
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking

    char data;
    int ret, angle, distance;
    Status_e status = UNKNOWN;
    while (1) {
        ret = read (fd, &data, 1);  // read 1 character if ready to read
        switch (status) {
        case UNKNOWN:
            if ((ret != 0) && (data == '\n')) {
                status = ANGLE;
                angle = 0;
            }
            break;
        case ANGLE:
            if (ret != 0) {
                if (data == ' ') {
                    status = DISTANCE;
                    distance = 0;
                    printf("%d ", angle);
                } else if ((data >= '0') && (data <= '9')){
                    angle = angle * 10 + (data - '0');
                }
            }
            break;
        case DISTANCE:
            if (ret != 0) {
                if (data == '\n') {
                    status = ANGLE;
                    printf("%d\n", distance);

                    x[angle] = distance * cos(-angle * M_PI / (2.0 * 180.0)); // lidar handles angle clockwise
                    y[angle] = distance * sin(-angle * M_PI / (2.0 * 180.0));
                    angle = 0;
                } else if ((data >= '0') && (data <= '9')){
                    distance = distance * 10 + (data - '0');
                }
            }
            break;
        default:
            break;
        }

        if (angle == ANGLE_MAX - 1) {
            gnuplot_resetplot(h);
            gnuplot_plot_xy(h, x, y, ANGLE_MAX, "map");
        }
    }

    gnuplot_close(h);

    return 0;
}
