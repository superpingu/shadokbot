#ifndef SERIAL_ARDUINO_IMPL_H
#define SERIAL_ARDUINO_IMPL_H

#include "serial.h"

namespace serial {
  using std::string;
  class serial::Serial::SerialImpl {
  public:
    SerialImpl(const string &port,
      unsigned long baudrate,
      bytesize_t bytesize,
      parity_t parity,
      stopbits_t stopbits,
      flowcontrol_t flowcontrol);

      bool open ();
      void close ();
      bool isOpen () const;
      size_t available ();
      bool getCTS ();
      bool getCD ();
      bool getRI ();
      bool getDSR ();
      bool setRTS (bool level);
      bool setDTR (bool level);

      parity_t getParity () const;
      bool setParity (parity_t parity);

      string getPort () const;
      void setPort (const string &port);

      Timeout getTimeout () const;
      void setTimeout (Timeout &timeout);

      flowcontrol_t getFlowcontrol () const;
      bool setFlowcontrol (flowcontrol_t flowcontrol);

      bool setBaudrate (unsigned long baudrate);
      unsigned long getBaudrate () const;

      bytesize_t getBytesize () const;
      bool setBytesize (bytesize_t bytesize);

      stopbits_t getStopbits () const;
      bool setStopbits (stopbits_t stopbits);

      void waitByteTimes (size_t count);
      bool waitForChange ();
      int waitfordata(size_t data_count, uint32_t timeout, size_t * returned_size);
      bool waitReadable (uint32_t timeout);

      void flush ();
      void flushInput ();
      void flushOutput ();


      size_t write (const uint8_t *data, size_t length);
      int writeLock ();
      int writeUnlock ();

      size_t read (uint8_t *buf, size_t size = 1);
      int readLock ();
      int readUnlock ();

      bool setBreak (bool level);
      void sendBreak (int duration);

    private:
      parity_t parity_;           // Parity
      bytesize_t bytesize_;       // Size of the bytes
      stopbits_t stopbits_;       // Stop Bits
      flowcontrol_t flowcontrol_; // Flow Control
  };
}

#endif
