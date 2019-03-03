#ifndef OBSTACLE_TEST_H
#define OBSTACLE_TEST_H

#include <string>
#include <cppunit/TestCase.h>

class ObstacleTest : public CppUnit::TestCase {
  public:
    ObstacleTest(std::string name);
    void runTest();
};

#endif // OBSTACLE_TEST_H
