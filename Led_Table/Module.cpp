#include "Module.h"

void Module::init(byte SENSOR_PIN) {
  this->SENSOR_PIN = SENSOR_PIN;
  pinMode(SENSOR_PIN, INPUT);
}


void Module::sendDataViaSerial() {
  for (int i = 0; i < 25; i++)
    Serial.write(readings[i]);
}

void Module::collect_data() {
  for (int i = 0; i < 25; i++) {
    PORTB = i; //PORTB - 8...13 pin
    if (!(i % 8)) delayMicroseconds(200);
    readings[i] = analogRead(SENSOR_PIN) / 4;
      //delay(100);// for debugging
  }
}
