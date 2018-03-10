#ifndef SWITCH_H
#define SWITCH_H

#include "Arduino.h"

class Switch { // is a 3 position lever-type switch

  private: byte SWITCH_PIN;
    //close to GND - ONE, MIDPOINT - TWO, VCC - THREE
  public: enum states {ONE, TWO, THREE};

  private: states state;

  private: states previousState;

  public:  Switch(byte SWITCH_PIN);

  public:  states readSwitchState();

  public:  states getState();

  public:  states getPreviousState();
};

#endif
