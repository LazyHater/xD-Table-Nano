/*
 * KMeans.h
 *
 *  Created on: 07.01.2017
 *      Author: Wakabajaszi
 */

#ifndef KMEANS_H_
#define KMEANS_H_

#include "Centroid.h"
#include "Graphics.h"
class KMeans {
byte k;
byte* readings;
Centroid centroids[4];
public:
	bool done = false;
	
	KMeans(byte* readings, byte k);
	void assignPointsToCentroids();
	void reposCentroids();
	void iterate();
	void resetCentroids();
	void setK(byte _k);
	float calcSSE();
	String toString();
	
};

#endif /* KMEANS_H_ */
