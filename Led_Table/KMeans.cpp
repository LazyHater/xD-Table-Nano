/*
 * KMeans.cpp
 *
 *  Created on: 07.01.2017
 *      Author: Wakabajaszi
 */

#include "KMeans.h"

KMeans::KMeans(byte* readings, byte k) : readings(readings), k(k) {
	resetCentroids();
}

void KMeans::assignPointsToCentroids(){
this->done = true;
	for(byte y = 0; y < 10; y++)
		for(byte x = 0; x < 10; x++){
			if(readings[x + y * 10]){
				float best = 100000;
				for(byte i = 0; i < k; i++){
			//		Serial.print(best);
		//			Serial.print(" Dist: ");
					
					float dist = distSqr(x, y, centroids[i].x, centroids[i].y);
	//				Serial.print(dist);
					
					if(dist < best) {
						readings[i] = i + 1;
						best = dist;
						done = false;
						//Serial.println("done");
					}	
				}				
			}
	}
}

void KMeans::reposCentroids(){
for(byte i = 0; i < k; i++){
	float avg_x = 0;
   	float avg_y = 0;
   	float count = 0;
	for(byte y = 0; y < 10; y++)
		for(byte x = 0; x < 10; x++){
			if(readings[x + y * 10]==i+1){
			
      avg_x += x;
      avg_y += y;
      count++;
      }
    }
    avg_x /= count;
    avg_y /= count;
    centroids[i].x =(byte)avg_x;
    centroids[i].y =(byte)avg_y;
	}
}

void KMeans::iterate(){
	assignPointsToCentroids();
	reposCentroids();
}

void KMeans::resetCentroids(){
	done = false;
	
	switch(k) {
    case 1:
      centroids[0].set(4, 4);
      break;
    case 2:
      centroids[0].set(3, 3);
      centroids[1].set(6, 6);
      break;
    case 3:
      centroids[0].set(3, 3);
      centroids[1].set(6, 3);
      centroids[2].set(5, 6);
      break;
    case 4:

      centroids[0].set(2, 2);
      centroids[1].set(7, 2);
      centroids[2].set(2, 7);
      centroids[3].set(7, 7);
      break;
    }
}

void KMeans::setK(byte _k){
	k = _k;
}

float KMeans::calcSSE(){
	float SSE = 0;
	for(byte i = 0; i < k; i++)
		for(byte y = 0; y < 10; y++)
			for(byte x = 0; x < 10; x++)
				if(readings[x + y * 10]==i+1)
					SSE += distSqr(x, y, centroids[i].x, centroids[i].y);
	return SSE;
}


	String KMeans::toString(){
	String s = "{";
	for(int i = 0; i < k; i ++){
		Serial.print("[");
		Serial.print(centroids[i].x);
		Serial.print(",");
		Serial.print(centroids[i].x);
		Serial.print("]");
		if(i != k - 1)
			Serial.print(",");
		}
Serial.println("}");
		return s; 
	}
