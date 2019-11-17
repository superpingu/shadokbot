#include "lidar/lidar.hpp"

Lidar::Lidar(Map *pMap) :
    buffer(2000),
    map(pMap)
{
}

Lidar::~Lidar()
{

}

void Lidar::pushSampleData(uint8_t data) {}
void Lidar::init(HardwareSerial& serial, int baudrate) {}
void Lidar::startScan() {}
void Lidar::readCb() {}
void Lidar::update() {}

void Lidar::updateMap() {}
int32_t Lidar::computeAngCorr(uint32_t distance) {return 0;}
uint16_t Lidar::convertAngle(int32_t angle) {return angle;}

void Lidar::parseFrame() {}
bool Lidar::needParsing() {return false;}
