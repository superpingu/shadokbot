#include "obstacleTest.h"
#include "obstacle.h"

ObstacleTest::ObstacleTest(std::string name): CppUnit::TestCase(name) {}

void ObstacleTest::runTest() {
  Obstacle obs(0, 1, -2, 0, 5);
  CPPUNIT_ASSERT(obs.distance(0,0) == 2);
}
