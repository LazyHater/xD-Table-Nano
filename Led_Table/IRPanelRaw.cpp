/*
 * IRPanelRaw.cpp
 *
 *  Created on: 03.01.2017
 *      Author: Wakabajaszi
 */


#include "IRPanelRaw.h"

IRPanelRaw::IRPanelRaw(byte F_PIN, byte G_PIN, byte INH_PIN, byte SENSOR_PIN):
		F_PIN(F_PIN), G_PIN(G_PIN), INH_PIN(INH_PIN) {

  pinMode(INH_PIN, OUTPUT); // firstly deal with disabling ir shiet
  digitalWrite(INH_PIN, LOW);

  for (int i = 0; i < 4; i++)
    modules[i].init(SENSOR_PIN); // initialize modules

  for (int i = 8; i < 13; i++)
    pinMode(i, OUTPUT); //sets all pins from 8 to 12 to output

  pinMode(F_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  digitalWrite(F_PIN, LOW);
  digitalWrite(G_PIN, LOW);

 // for (int i = 0; i < 100; i++) // for debug
 //   readings[i] = i;
}

void IRPanelRaw::collectData() {
  digitalWrite(INH_PIN, LOW); //enable use power for ir
  for (int i = 0; i < 4; i++) {
    selectModule(i);
    delayMicroseconds(100); //give some time for 4052 & 4051 chips
    modules[i].collect_data();
  }
  digitalWrite(INH_PIN, HIGH); //disable use power for ir
  fillBigTab(readings); // rewrite values from 4 smaller tabs to one big
}

void IRPanelRaw::sendDataViaSerial() {
  for (int i = 0; i < 100; i++)
 //   Serial.write(abs(readings[i] - refference_table[i]));
   Serial.write(readings[i]);
}

void IRPanelRaw::fillBigTab(byte tab[100]) {
  byte i, j;
  for (j = 0; j < 25; j += 5)
    for (i = 0; i < 5; i++) {
      tab[i + (j << 1)] = modules[0].readings[i + j];
      tab[i + (j << 1) + 5] = modules[1].readings[i + j];
      tab[i + (j << 1) + 50] = modules[2].readings[i + j];
      tab[i + (j << 1) + 55] = modules[3].readings[i + j];
    }
}

byte IRPanelRaw::getValueAt(byte idx){
	return readings[idx];
}

byte IRPanelRaw::getValueAt(byte x, byte y){
	return readings[x+10*y];
}


//modules layout by id:
// 	0	1
//	2	3

void IRPanelRaw::selectModule(byte id) {
  switch (id) {
    case 0:
      digitalWrite(F_PIN, LOW);
      digitalWrite(G_PIN, LOW);
      break;
    case 1:
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, LOW);
      break;
    case 2:
      digitalWrite(F_PIN, LOW);
      digitalWrite(G_PIN, HIGH);
      break;
    case 3:
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;
  }
}

void IRPanelRaw::createRefferenceTable(byte how_many_averages) { // averages readings and stores it in array
  for (int i = 0; i < 100; i++) // fill reference table with zeros
    refference_table[i] = 0;

  for (int j = 0; j < how_many_averages; j++) { // sum iterations
    collectData();
    for (int i = 0; i < 100; i++)
      refference_table[i] += readings[i];
  }

  for (int i = 0; i < 100; i++) // divade all by number of additions
    refference_table[i] /= how_many_averages;
}

