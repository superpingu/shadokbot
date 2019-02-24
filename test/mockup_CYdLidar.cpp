#define YDLIDAR_API
#include <CYdLidar.h>

CYdLidar::CYdLidar() {}

CYdLidar::~CYdLidar() {};

bool CYdLidar::initialize() {
  return false;
}

bool CYdLidar::doProcessSimple(LaserScan &outscan, bool &hardwareError) {
  hardwareError = false;
  for (int i = 0; i < 720; i++) {
    outscan.ranges.push_back(1.0);
  }
  outscan.ranges[0] = 1.6;
  outscan.ranges[2] = 1.7;

  return true;
}

bool CYdLidar::turnOn() {
  return false;
}

bool CYdLidar::turnOff() {
  return false;
}

bool CYdLidar::getDeviceHealth() const {
  return false;
}

bool CYdLidar::getDeviceInfo(int &type) {
  return false;
}

bool CYdLidar::checkHeartBeat() const {
  return false;
}

bool CYdLidar::checkScanFrequency() {
  return false;
}

void CYdLidar::disconnecting() {};


bool CYdLidar::checkCOMMs() {
  return false;
}

bool CYdLidar::checkStatus() {
  return false;
}

bool CYdLidar::checkHardware() {
  return false;
}
