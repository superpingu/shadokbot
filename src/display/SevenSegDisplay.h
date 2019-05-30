#ifndef SevenSegDisplay_h
#define SevenSegDisplay_h

#include <Arduino.h>

#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0

class SevenSegDisplay {
 public:
  SevenSegDisplay(void);
  void begin(uint8_t _addr = 0x70);
  void setBrightness(uint8_t b);
  void blinkRate(uint8_t b);
  void writeDisplay(void);
  void clear(void);

  void show(int value);

  uint16_t displaybuffer[8];

  void init(uint8_t a);
 protected:
  uint8_t i2c_addr;
};

extern SevenSegDisplay display;

#endif // Adafruit_LEDBackpack_h
