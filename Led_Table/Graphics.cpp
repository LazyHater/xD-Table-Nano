/*
 * Graphics.cpp
 *
 *  Created on: 06.01.2017
 *      Author: Wakabajaszi
 */

#include "Graphics.h"

uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return Led_Table_Pixels::Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return Led_Table_Pixels::Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Led_Table_Pixels::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setRow(Led_Table_Pixels *strip, byte row, uint32_t color) {
	for (int i = 0; i < 10; i++)
		strip->setPixelColor(i, row, color);
}

void setCol(Led_Table_Pixels *strip, byte col, uint32_t color) {
	for (int i = 0; i < 10; i++)
		strip->setPixelColor(i, col, color);
}

void drawLine(Led_Table_Pixels *strip, int x1, int y1, int x2, int y2,
		uint32_t c) {
	// zmienne pomocnicze
	int d, dx, dy, ai, bi, xi, yi;
	int x = x1, y = y1;
	// ustalenie kierunku rysowania
	if (x1 < x2) {
		xi = 1;
		dx = x2 - x1;
	} else {
		xi = -1;
		dx = x1 - x2;
	}
	// ustalenie kierunku rysowania
	if (y1 < y2) {
		yi = 1;
		dy = y2 - y1;
	} else {
		yi = -1;
		dy = y1 - y2;
	}
	// pierwszy piksel
	strip->setPixelColor(x, y, c);
	// oœ wiod¹ca OX
	if (dx > dy) {
		ai = (dy - dx) * 2;
		bi = dy * 2;
		d = bi - dx;
		// pêtla po kolejnych x
		while (x != x2) {
			// test wspó³czynnika
			if (d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			} else {
				d += bi;
				x += xi;
			}
			strip->setPixelColor(x, y, c);
		}
	}
	// oœ wiod¹ca OY
	else {
		ai = (dx - dy) * 2;
		bi = dx * 2;
		d = bi - dy;
		// pêtla po kolejnych y
		while (y != y2) {
			// test wspó³czynnika
			if (d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			} else {
				d += bi;
				y += yi;
			}
			strip->setPixelColor(x, y, c);
		}
	}
}

uint32_t getRandomColor(byte min_brightness) {
	uint32_t color = 0;

	byte r = random(255 - min_brightness) + min_brightness;
	byte g = random(255 - min_brightness) + min_brightness;
	byte b = random(255 - min_brightness) + min_brightness;

	return ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
}

float distSqr(byte x0, byte y0, byte x1, byte y1) { ///// TEST IT!!!
	return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
}
