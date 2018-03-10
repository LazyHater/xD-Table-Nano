#include "Switch.h"


Switch::Switch(byte SWITCH_PIN): SWITCH_PIN(SWITCH_PIN), state(ONE), previousState(ONE){
  pinMode(this->SWITCH_PIN, INPUT);
}

Switch::states Switch::readSwitchState() {
  previousState = state;
  int value = analogRead(this->SWITCH_PIN);
  if (value <= 340) {
    state = ONE;
    return state;
  } else if ((value > 340) && (value <= 680)) {
    state = TWO;
    return state;
  } else { // (value > 680)
    state = THREE;
    return state;
  }
}

Switch::states Switch::getState() {
	return state;
}

Switch::states Switch::getPreviousState() {
	return previousState;
}
