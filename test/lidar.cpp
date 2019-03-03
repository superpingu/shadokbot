#include <CYdLidar.h>
#include <obstacle.h>
#include <lidar.h>
#include "obstacleTest.h"
#include <cppunit/ui/text/TestRunner.h>

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


  std::tuple<int,float> point = closest_obstacle(scan.ranges);
  printf("closest point is %d at %f\n", std::get<0>(point), std::get<1>(point));

  laser.turnOff();
  laser.disconnecting();

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(ObstacleTest::suite());
  runner.run();

  return 0;
}
