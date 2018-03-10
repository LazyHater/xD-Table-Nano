/*
 * IRPanel2.h
 *
 *  Created on: 03.01.2017
 *      Author: Wakabajaszi
 */

#ifndef IRPANEL_H_
#define IRPANEL_H_

#include "Arduino.h"
#include "KMeans.h"
#include "IRPanelRaw.h"

class IRPanel : public IRPanelRaw{
  protected:
	byte threshold;
	KMeans kMeans;
  public:
    IRPanel(byte F_PIN, byte G_PIN, byte INH_PIN, byte SENSOR_PIN, byte threshold):
    	IRPanelRaw( F_PIN, G_PIN, INH_PIN, SENSOR_PIN), threshold(threshold),
		kMeans((byte*)this->readings, 1){};
    	
    	//0 - not covered sensor
    	//1 - covered sensor
    void convertReadingsToBoll(){
    	for(byte i = 0; i < 100; i++){
    		if(abs(readings[i] - refference_table[i]) >= threshold){
    			readings[i] = 1;	
    		} else {
    			readings[i] = 0;
    		}
    	}
    }
    
    void setThreshold(byte t){
    	threshold = t;
    }
    
    byte getThreshold(){
    	return threshold;
    }

    void resolveKMeans(){
    	kMeans.setK(1);
    	kMeans.resetCentroids();
    	Serial.println("entering");

    	int max_iterations = 10;
    	while((!kMeans.done)&&(max_iterations > 0)){
    		Serial.println("computing");
    		kMeans.iterate();
    		max_iterations--;
    	}
    	kMeans.toString();
    }

};

#endif /* IRPanel_H_ */
