#ifndef OBSTACLE_TEST_H
#define OBSTACLE_TEST_H

#include <string>
#include "obstacle.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/Test.h>

class ObstacleTest : public CppUnit::TestFixture {
  public:
    ObstacleTest();
    void setUp();
    void tearDown();
    void testDistanceHorizontal();
    void testDistanceVertical();
    void testDistanceDefault();
    static CppUnit::Test *suite();

  private:
    Obstacle *obs1, *obs2, *obs3;
};

#endif // OBSTACLE_TEST_H
