#ifndef IRPanelRaw_H
#define IRPanelRaw_H

#include "Arduino.h"
#include "Module.h"

class IRPanelRaw {
  private:
    Module modules[4];
    byte F_PIN;
    byte G_PIN;
    byte INH_PIN;
    
    void fillBigTab(byte tab[100]);
    
    void selectModule(byte id);

  protected:
    int refference_table[100] = {0};
    byte readings[100] = {0};
	
  public:
    IRPanelRaw(byte F_PIN, byte G_PIN, byte INH_PIN, byte SENSOR_PIN);

    void collectData();

    void createRefferenceTable(byte how_many_averages = 10);     

    void sendDataViaSerial();
    
    byte getValueAt(byte idx);
    
    byte getValueAt(byte x, byte y);
};




#endif
