#define YDLIDAR_API
#include <CYdLidar.h>

CYdLidar::CYdLidar() {
  LaserScan scan;
  scan.config.foo = NULL;
  (void)scan;
}

CYdLidar::~CYdLidar() {};

bool CYdLidar::initialize(){return false;};
bool CYdLidar::doProcessSimple(LaserScan &outscan, bool &hardwareError){return false;};
bool CYdLidar::turnOn() {return false;};
bool CYdLidar::turnOff() {return false;};
bool CYdLidar::getDeviceHealth() const {return false;};
bool CYdLidar::getDeviceInfo(int &type) {return false;};
bool CYdLidar::checkHeartBeat() const {return false;};
bool CYdLidar::checkScanFrequency() {return false;};
void CYdLidar::disconnecting() {};

bool CYdLidar::checkCOMMs() {return false;};
bool CYdLidar::checkStatus(){return false;};
bool CYdLidar::checkHardware() {return false;};
