/*
 * EffectMenager.h
 *
 *  Created on: 06.01.2017
 *      Author: Wakabajaszi
 */

#ifndef EFFECTMENAGER_H_
#define EFFECTMENAGER_H_

#include "Effects.h"

#include "Led_Table_Pixels.h"
#include "IRPanel.h"

#define NUMB_OF_EFFECTS 8
#define STARTING_EFFECT 8

class EffectMenager {
Blank blank;
Rainbow rainbow;
RainbowLine rainbowLine;
RainbowFade rainbowFade;
RandomPixels randomPixels;
Rain rain;
MetaBalls metaBalls;
Tennis tenis;

Effect* activeEffect;
Effect* tableOfEffects[NUMB_OF_EFFECTS];

public:
	EffectMenager(Led_Table_Pixels* strip, IRPanel* irPanel);
	void nextFrame();
	void show();
	void nextEffect();
};

#endif /* EFFECTMENAGER_H_ */
