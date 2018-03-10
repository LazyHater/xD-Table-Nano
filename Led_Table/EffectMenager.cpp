/*
 * EffectMenager.cpp
 *
 *  Created on: 06.01.2017
 *      Author: Wakabajaszi
 */

#include "EffectMenager.h"



EffectMenager::EffectMenager(Led_Table_Pixels* strip, IRPanel* irPanel):
	rainbow(strip, irPanel), blank(strip, irPanel),
	rainbowLine(strip, irPanel), rainbowFade(strip, irPanel),
	randomPixels(strip, irPanel), rain(strip, irPanel),
	metaBalls(strip, irPanel), tenis(strip, irPanel){

	tableOfEffects[0] = &blank;
	tableOfEffects[1] = &rainbow;
	tableOfEffects[2] = &rainbowFade;
	tableOfEffects[3] = &randomPixels;
	tableOfEffects[4] = &rainbowLine;
	tableOfEffects[5] = &rain;
	tableOfEffects[6] = &metaBalls;
	tableOfEffects[7] = &tenis;

 	activeEffect = (tableOfEffects[STARTING_EFFECT - 1]);
}
	// TODO Auto-generated constructor stub
void EffectMenager::nextFrame(){
	activeEffect->nextFrame();
}

void EffectMenager::show(){
	activeEffect->show();
}

void EffectMenager::nextEffect(){
	static byte current_effect_id = STARTING_EFFECT - 1;
	current_effect_id++;
	if(current_effect_id > NUMB_OF_EFFECTS - 1)
		current_effect_id = 0;
	activeEffect = tableOfEffects[current_effect_id];
}


