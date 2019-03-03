#include "obstacleTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

ObstacleTest::ObstacleTest(): CppUnit::TestFixture() {}

void ObstacleTest::setUp() {
  obs1 = new Obstacle(0, 1, -2, 0, 5);
  obs2 = new Obstacle(1.5, -1, 1.5, 1, 3);
  obs3 = new Obstacle(1, 0, 5, 1, 3);
}

void ObstacleTest::tearDown() {
  delete obs1;
  delete obs2;
  delete obs3;
}

void ObstacleTest::testDistanceHorizontal() {
  CPPUNIT_ASSERT(obs1->distance(0,0) == 2);
  CPPUNIT_ASSERT(obs1->distance(10,2) == 5);
  CPPUNIT_ASSERT(obs1->distance(5,0) == 2);
  CPPUNIT_ASSERT(obs1->distance(2.5,0.5) == 1.5);
}

void ObstacleTest::testDistanceDefault() {
  CPPUNIT_ASSERT(obs2->distance(0,0) > 3.162);
  CPPUNIT_ASSERT(obs2->distance(0,0) < 3.163);
}

void ObstacleTest::testDistanceVertical() {
  CPPUNIT_ASSERT(obs3->distance(0,1) == 5);
}

CppUnit::Test* ObstacleTest::suite() {
  CppUnit::TestSuite *suiteOfTest = new CppUnit::TestSuite("Obstacle test");
  suiteOfTest->addTest(new CppUnit::TestCaller<ObstacleTest> ("test distance", &ObstacleTest::testDistanceHorizontal));
  suiteOfTest->addTest(new CppUnit::TestCaller<ObstacleTest> ("test distance", &ObstacleTest::testDistanceVertical));
  suiteOfTest->addTest(new CppUnit::TestCaller<ObstacleTest> ("test distance", &ObstacleTest::testDistanceDefault));
  return suiteOfTest;
}
