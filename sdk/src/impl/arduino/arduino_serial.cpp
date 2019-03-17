#include "arduino_serial.h"

namespace serial {
Serial::SerialImpl::SerialImpl(const string &port,
  unsigned long baudrate,
  bytesize_t bytesize,
  parity_t parity,
  stopbits_t stopbits,
  flowcontrol_t flowcontrol) {};

  bool Serial::SerialImpl::open () {return false;};
  void Serial::SerialImpl::close () {};
  bool Serial::SerialImpl::isOpen () const{return false;};
  size_t Serial::SerialImpl::available (){return 0;};
  bool Serial::SerialImpl::getCTS (){return false;};
  bool Serial::SerialImpl::getCD (){return false;};
  bool Serial::SerialImpl::getRI (){return false;};
  bool Serial::SerialImpl::getDSR (){return false;};
  bool Serial::SerialImpl::setRTS (bool level){return false;};
  bool Serial::SerialImpl::setDTR (bool level){return false;};

  parity_t Serial::SerialImpl::getParity () const{return parity_none;};
  bool Serial::SerialImpl::setParity (parity_t parity){return false;};

  string Serial::SerialImpl::getPort () const{return "";};
  void Serial::SerialImpl::setPort (const string &port){};

  Timeout Serial::SerialImpl::getTimeout () const{return Timeout::simpleTimeout(0);};
  void Serial::SerialImpl::setTimeout (Timeout &timeout){};

  flowcontrol_t Serial::SerialImpl::getFlowcontrol () const {return flowcontrol_none;};
  bool Serial::SerialImpl::setFlowcontrol (flowcontrol_t flowcontrol) {};

  unsigned long Serial::SerialImpl::getBaudrate () const {return 115200;};
  bool Serial::SerialImpl::setBaudrate (unsigned long baudrate) {return false;};

  bytesize_t Serial::SerialImpl::getBytesize () const {return fivebits;};
  bool Serial::SerialImpl::setBytesize (bytesize_t bytesize){return false;};

  stopbits_t Serial::SerialImpl::getStopbits () const {return stopbits_one;};
  bool Serial::SerialImpl::setStopbits (stopbits_t stopbits){return false;};

  void Serial::SerialImpl::waitByteTimes (size_t count) {};
  bool Serial::SerialImpl::waitForChange (){return false;};
  int Serial::SerialImpl::waitfordata(size_t data_count, uint32_t timeout, size_t * returned_size) {return 0;};
  bool Serial::SerialImpl::waitReadable (uint32_t timeout){return false;};

  void Serial::SerialImpl::flush () {};
  void Serial::SerialImpl::flushInput () {};
  void Serial::SerialImpl::flushOutput () {};

  size_t Serial::SerialImpl::write (const uint8_t *data, size_t length) {return 0;};
  int Serial::SerialImpl::writeLock () {return 0;};
  int Serial::SerialImpl::writeUnlock () {return 0;};

  size_t Serial::SerialImpl::read (uint8_t *buf, size_t size) {return 0;};
  int Serial::SerialImpl::readLock () {return 0;};
  int Serial::SerialImpl::readUnlock () {return 0;};

  bool Serial::SerialImpl::setBreak (bool level) {return false;};
  void Serial::SerialImpl::sendBreak (int duration) {};
}
