
#ifndef LED_TABLE_PIXELS_H
#define LED_TABLE_PIXELS_H

#include "Adafruit_NeoPixel.h"

#define IDX(x,y) ((x)+10*(y))

class Led_Table_Pixels : public Adafruit_NeoPixel{
private:

const byte reshuffle_tab[100] = {
  0,  1,  2,  3,  4,   25, 26, 27, 28, 29,
  5,  6,  7,  8,  9,   30, 31, 32, 33, 34,
  10, 11, 12, 13, 14,  35, 36, 37, 38, 39,
  15, 16, 17, 18, 19,  40, 41, 42, 43, 44,
  20, 21, 22, 23, 24,  45, 46, 47, 48, 49,

  50, 51, 52, 53, 54,  75, 76, 77, 78, 79,
  55, 56, 57, 58, 59,  80, 81, 82, 83, 84,
  60, 61, 62, 63, 64,  85, 86, 87, 88, 89,
  65, 66, 67, 68, 69,  90, 91, 92, 93, 94,
  70, 71, 72, 73, 74,  95, 96, 97, 98, 99,
};

public:
	Led_Table_Pixels(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800);

	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    
    void setPixelColor(uint16_t n, uint32_t c);

	void setPixelColor(byte x, byte y, uint8_t r, uint8_t g, uint8_t b);
   
	void setPixelColor(byte x, byte y, uint32_t c);

	void fade(float strenght, byte minimal_brightness = 0);

	void background(uint32_t c);
	
	void background(uint8_t r, uint8_t g, uint8_t b);

	byte getPixelBrightness(byte x, byte y);
	
	byte getPixelBrightness(byte n);
	
	uint32_t getPixelColor(byte x, byte y) const;
	
	uint32_t getPixelColor(uint16_t n) const;
};

#endif // LED_TABLE_PIXELS_H

