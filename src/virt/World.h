#pragma once

#include "ofMain.h"
#include "Entity.h"

class World {
public:
	static void setup(ofPixels *heightMap);

	static ofPixels* getHeightMap();
	static ofVec2f getSize();

	static double getGravity();
	static double getFriction();
	static ofVec2f getWorldToMapRatio();
	static ofVec2f getMapPos(ofVec2f pos);
	static double getAverageHeight(ofVec2f pos, double size);
	static double getZ(ofVec2f pos);
private:

	static ofPixels* heightMap;
	static ofVec2f size;

	static double gravity;
	static double friction;
};