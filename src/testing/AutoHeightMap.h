#pragma once

#include "ofMain.h"

class AutoHeightMap {
public:
	void setup(int width, int height);
	void update();
	void draw(int x, int y, int width, int height);
	ofPixels* getPixelsRef();
	
	
private:
	ofImage heightMap;
	
	double scaleX;
	double scaleY;
	double z;
};