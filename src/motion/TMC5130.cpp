#include "./SPI.h"
#include "Arduino.h"
#include "TMC5130-constants.h"
#include "TMC5130.hpp"
#include "motionconf.h"

TMC5130::TMC5130(bool _inv_direction, int _cs_pin, int _int_pin) {
  cs_pin = _cs_pin;
  int_pin = _int_pin;

  SPI.begin();

  digitalWrite(cs_pin, HIGH);
  pinMode(cs_pin, OUTPUT);
  pinMode(int_pin, INPUT);

  // set ramp generator target and actual to 0 before activating the driver
  xtarget = xmovestart = 0;
  write_data(TMC_XACTUAL, 0);
  write_data(TMC_XTARGET, 0);
  // Tzerowait = 300ms
  write_data(TMC_TZEROWAIT, 200*(TMC5130_CLKFREQ/1000)/512);
  // position mode
  write_data(TMC_RAMPMODE, TMC_MODE_POSITION);

  // enable stealthchop, inverse motor rotation direction if required
  write_data(TMC_GCONF, (_inv_direction ? 0x10 : 0) + 0x04);
  // current limits: Ihold = 0, Irun = 1A -> 24, IHOLDDELAY = 1
  write_data(TMC_IHOLD_IRUN, 0x1600);
  // powerdown delay = 500ms
  write_data(TMC_TPOWERDOWN, 500*(TMC5130_CLKFREQ/1000)/262144);
  // stealthchop/spreadcycle switching speed = 0.5turn/s
  write_data(TMC_TPWMTHRS, TMC5130_CLKFREQ/(128*STEP_PER_TURN));
  // stealthchop configuration: 35kHz PWM with 12MHz clock, FREEWHEEL=LS short, PWM_GRAD=6, PWM_AMPL=127
  write_data(TMC_PWMCONF, 0x2504F0);
  // spreadcycle configuration: TOFF=4, TBL=2, HSTRT=6, HEND=0, spreadcycle enabled, vsense=0, no chopsync
  write_data(TMC_CHOPCONF, 0x40010064);

  // read ramp status to clear interrupt pin
  read_data(TMC_RAMPSTAT);
}

#define ustep_per_s2_to_tmcacc(x) (( ((uint32_t) x)*((268435456/TMC5130_CLKFREQ)*(268435456/TMC5130_CLKFREQ)) ) >> 15)

void TMC5130::setLowSpeedAcceleration(int acceleration, int deceleration) {
  write_data(TMC_A1, ustep_per_s2_to_tmcacc(acceleration));
  write_data(TMC_D1, ustep_per_s2_to_tmcacc(deceleration));
}
void TMC5130::setHighSpeedAcceleration(int acceleration, int deceleration) {
  write_data(TMC_AMAX, ustep_per_s2_to_tmcacc(acceleration));
  write_data(TMC_DMAX, ustep_per_s2_to_tmcacc(deceleration));
}

#define ustep_per_s_to_tmcspeed(x) (( ((uint32_t) x)*(1073741824/TMC5130_CLKFREQ) ) >> 6)

void TMC5130::setMinSpeed(int startspeed, int stopspeed) {
  uint32_t tmcstopspeed = ustep_per_s_to_tmcspeed(stopspeed);
  write_data(TMC_VSTART, ustep_per_s_to_tmcspeed(startspeed));
  write_data(TMC_VSTOP, tmcstopspeed < 10 ? 10 : tmcstopspeed);
}
void TMC5130::setTransitionSpeed(int speed) {
  write_data(TMC_V1, ustep_per_s_to_tmcspeed(speed));
}
void TMC5130::setCruiseSpeed(int speed) {
  write_data(TMC_VMAX, ustep_per_s_to_tmcspeed(speed));
}

#define tmcspeed_to_ustep_per_s(x) ((x*(TMC5130_CLKFREQ/65536)) >> 8)

int TMC5130::getSpeed() {
  int speed = (int) read_data(TMC_VACTUAL);
  return tmcspeed_to_ustep_per_s(speed);
}

void TMC5130::setTargetPosition(int position) {
  moving = xtarget != position;
  xtarget = position;
  write_data(TMC_XTARGET, position);
}
int TMC5130::getPosition() {
  int position;
  // query the position only if it is changing, otherwise return the last goal value
  if(moving)
    position = (int) read_data(TMC_XACTUAL);
  else
    position = xtarget;
  return position;
}
void TMC5130::move(int distance) {
  xmovestart = xtarget;
  setTargetPosition(xtarget+distance);
}
int TMC5130::getMovePosition() {
  return getPosition() - xmovestart;
}
int TMC5130::getMoveGoal() {
  return xtarget - xmovestart;
}

bool TMC5130::isMoving() {
  // we are moving and the interrupt pin is asserted, read interrupt status
  if(moving && digitalRead(int_pin) == LOW) {
    uint32_t ramp_status = read_data(TMC_RAMPSTAT);
    moving = !(ramp_status & TMC_RS_POSREACHED);
  }
  return moving;
}

void TMC5130::emergencyStop() {

}
void TMC5130::emergencyResume() {

}

void TMC5130::write_data(uint8_t address, uint32_t data) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(cs_pin, LOW);
  delayMicroseconds(5);

  SPI.transfer(address | 0x80); // address MSbit = 1 -> write
  SPI.transfer((data >> 24) & 0xff);
  SPI.transfer((data >> 16) & 0xff);
  SPI.transfer((data >> 8) & 0xff);
  SPI.transfer((data) & 0xff);

  digitalWrite(cs_pin, HIGH);
  delayMicroseconds(5);
  SPI.endTransaction();
}

uint32_t TMC5130::read_data(uint8_t address) {
  unsigned long rx_data;

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(cs_pin, LOW);
  delayMicroseconds(5);

  // first transfer: command, rx data ignored
  SPI.transfer(address); // address MSbit = 0 -> read
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);

  digitalWrite(cs_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(cs_pin, LOW);
  delayMicroseconds(5);

  // second transfer: actual data rx
  SPI.transfer(address);
  rx_data = SPI.transfer(0);
  rx_data <<= 8;
  rx_data |= SPI.transfer(0);
  rx_data <<= 8;
  rx_data |= SPI.transfer(0);
  rx_data <<= 8;
  rx_data |= SPI.transfer(0);

  digitalWrite(cs_pin, HIGH);
  delayMicroseconds(5);
  SPI.endTransaction();

  return rx_data;
}
