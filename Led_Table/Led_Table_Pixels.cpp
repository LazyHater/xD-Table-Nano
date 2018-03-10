#include "Led_Table_Pixels.h"

Led_Table_Pixels::Led_Table_Pixels(uint16_t n, uint8_t p, neoPixelType t):
  Adafruit_NeoPixel( n, p,  t)
{}

	void Led_Table_Pixels::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
		Adafruit_NeoPixel::setPixelColor(reshuffle_tab[n],r,g,b);
	}

    void Led_Table_Pixels::setPixelColor(uint16_t n, uint32_t c){
    	Adafruit_NeoPixel::setPixelColor(reshuffle_tab[n],c);
    }

	void Led_Table_Pixels::setPixelColor(byte x, byte y, uint8_t r, uint8_t g, uint8_t b){
		Adafruit_NeoPixel::setPixelColor(reshuffle_tab[IDX(x,y)],r,g,b);
	}

	void Led_Table_Pixels::setPixelColor(byte x, byte y, uint32_t c){
		Adafruit_NeoPixel::setPixelColor(reshuffle_tab[IDX(x,y)],c);
	}

	void Led_Table_Pixels::fade(float strenght, byte minimal_brightness){
		for(byte i = 0; i < numPixels(); i++){
			byte pix_bright = getPixelBrightness(i);

	    	if(pix_bright < minimal_brightness)
	    		continue;

	    	uint32_t c = getPixelColor(i);
	    	byte r = (byte)(((c&0x00ff0000)>>16)*strenght);
	    	byte g = (byte)(((c&0x0000ff00)>>8)*strenght);
	    	byte b = (byte)((c&0x000000ff)*strenght);
	    	setPixelColor(i, r, g, b);
		}
	}

	void Led_Table_Pixels::background(uint32_t c){
		if(c == 0){
			Adafruit_NeoPixel::clear();
			return;
		}
		for(byte i = 0; i < 100; i++)
			Adafruit_NeoPixel::setPixelColor(i,c);
	}

	void Led_Table_Pixels::background(uint8_t r, uint8_t g, uint8_t b){
		Led_Table_Pixels::background(((uint32_t)r << 16) | ((uint32_t)g <<  8) | b);
	}

	byte Led_Table_Pixels::getPixelBrightness(byte n){
			uint32_t c = getPixelColor(n);
			return (((c&0x00ff0000)>>16)+((c&0x0000ff00)>>8)+(c&0x000000ff))/3;
		}
	byte Led_Table_Pixels::getPixelBrightness(byte x, byte y){
			uint32_t c = getPixelColor(IDX(x,y));
			return (((c&0x00ff0000)>>16)+((c&0x0000ff00)>>8)+(c&0x000000ff))/3;
		}

	uint32_t Led_Table_Pixels::getPixelColor(uint16_t n) const{
		return Adafruit_NeoPixel::getPixelColor(reshuffle_tab[n]);
	}

	uint32_t Led_Table_Pixels::getPixelColor(byte x, byte y) const{
		return Adafruit_NeoPixel::getPixelColor(reshuffle_tab[IDX(x,y)]);
	}
