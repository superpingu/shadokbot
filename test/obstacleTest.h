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
    void testIntersect1();
    void testIntersect2();
    static CppUnit::Test *suite();

  private:
    Obstacle *obs1, *obs2, *obs3, *obs4, *obs5;
    Line *line1, *line2;
};

#endif // OBSTACLE_TEST_H
