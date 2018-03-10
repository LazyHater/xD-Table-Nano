#ifndef MODULE_H
#define MODULE_H

#include "Arduino.h"

class Module {
  private:
    byte SENSOR_PIN;

  public:
    byte readings[25] = {0};
    void collect_data();
    void sendDataViaSerial();
    void init(byte SENSOR_PIN);
};


#endif
