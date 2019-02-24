#include <CYdLidar.h>
#include <lidar.h>

CYdLidar laser;
LaserScan scan;

int main(int argc, char* argv[]) {
  bool error = false;

  laser.setSerialPort("");
  laser.setSerialBaudrate(128000);
  laser.setIntensities(0);

  laser.initialize();

  laser.doProcessSimple(scan, error);
  printf("invalid data: %d\n", clean_data(scan.ranges));

  laser.turnOff();
  laser.disconnecting();
  return 0;
}
