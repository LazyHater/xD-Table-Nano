#ifndef GRAPHICS
#define GRAPHICS

#include "Arduino.h"
#include "Led_Table_Pixels.h"

uint32_t Wheel(byte WheelPos);

void setRow(Led_Table_Pixels *strip,byte row, uint32_t color);

void setCol(Led_Table_Pixels *strip, byte col, uint32_t color);

void drawLine(Led_Table_Pixels *strip, int x1, int y1, int x2, int y2, uint32_t c);

uint32_t getRandomColor(byte min_brightness = 0);

float distSqr(byte x0, byte y0, byte x1, byte y1 );
#endif
