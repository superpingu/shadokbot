#include <Wire.h>
#include "SevenSegDisplay.h"

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

static const uint8_t numbertable[] = {
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};

void SevenSegDisplay::setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  Wire1.beginTransmission(i2c_addr);
  Wire1.write(HT16K33_CMD_BRIGHTNESS | b);
  Wire1.endTransmission();
}

void SevenSegDisplay::blinkRate(uint8_t b) {
  Wire1.beginTransmission(i2c_addr);
  if (b > 3) b = 0; // turn off if not sure

  Wire1.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
  Wire1.endTransmission();
}

SevenSegDisplay::SevenSegDisplay(void) {
}

void SevenSegDisplay::begin(uint8_t _addr) {
  i2c_addr = _addr;

  Wire1.begin();

  Wire1.beginTransmission(i2c_addr);
  Wire1.write(0x21);  // turn on oscillator
  Wire1.endTransmission();
  blinkRate(HT16K33_BLINK_OFF);

  setBrightness(1); // max brightness
}

void SevenSegDisplay::writeDisplay(void) {
  Wire1.beginTransmission(i2c_addr);
  Wire1.write((uint8_t)0x00); // start at address $00

  for (uint8_t i=0; i<8; i++) {
    Wire1.write(displaybuffer[i] & 0xFF);
    Wire1.write(displaybuffer[i] >> 8);
  }
  Wire1.endTransmission();
}

void SevenSegDisplay::clear(void) {
  for (uint8_t i=0; i<8; i++) {
    displaybuffer[i] = 0;
  }
}

void SevenSegDisplay::show(int value) {
    clear();
    for (int i = 4; i >= 0; i--) {
        if(i == 2) continue;

        displaybuffer[i] = numbertable[value % 10];
        value = value / 10;
    }
    writeDisplay();
}

SevenSegDisplay display;
