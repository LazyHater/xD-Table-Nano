/*
 * Centroid.h
 *
 *  Created on: 07.01.2017
 *      Author: Wakabajaszi
 */

#ifndef CENTROID_H_
#define CENTROID_H_

#include <Arduino.h>

class Centroid {

public:

byte x;
byte y;

	Centroid();
	void set(byte xx, byte yy);
	
};

#endif /* CENTROID_H_ */
