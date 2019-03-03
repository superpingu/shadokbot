#include <CYdLidar.h>
#include <obstacle.h>
#include <lidar.h>
#include "obstacleTest.h"

CYdLidar laser;
LaserScan scan;

void testObstacle() {
  Obstacle obs1(0, 1, -2, 0, 5);
  printf("Distance %f\n", obs1.distance(0,0));
  printf("Distance %f\n", obs1.distance(10,2));
  printf("Distance %f\n", obs1.distance(5,2));
  printf("Distance %f\n", obs1.distance(2.5,0.5));
  printf("\n");

  Obstacle obs2(1.5, -1, 1.5, 1, 3);
  printf("Distance %f\n", obs2.distance(0,0));
  printf("Distance %f\n", obs2.distance(2,1));
  printf("\n");

  Obstacle obs3(1, 0, 5, 1, 3, true);
  printf("Distance %f\n", obs3.distance(0,0));

}

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

  ObstacleTest test("Obstacle test");
  test.runTest();

  return 0;
}
