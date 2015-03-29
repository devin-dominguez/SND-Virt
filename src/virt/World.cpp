#include "World.h"

ofPixels* World::heightMap;
ofVec2f World::size = ofVec2f(800, 600);
double World::gravity = -981;
double World::friction = 1;

void World::setup(ofPixels* heightMap) {
	World::heightMap = heightMap;
}

ofPixels* World::getHeightMap() { return heightMap;}
ofVec2f World::getSize() { return size;}
double World::getGravity() { return gravity;}
double World::getFriction() { return friction;}


ofVec2f World::getWorldToMapRatio() {
	ofVec2f ratio;
	ratio.x = heightMap->getWidth() / size.x;
	ratio.y = heightMap->getHeight() / size.y;

	return ratio;
}

ofVec2f World::getMapPos(ofVec2f pos) {
	return pos * getWorldToMapRatio();
}

double World::getAverageHeight(ofVec2f pos, double size) {
	//determine average height from heightmap
	ofVec2f hmPos = getMapPos(pos);

	int w = heightMap->getWidth();
	ofVec2f scanRange = size * getWorldToMapRatio();
	double height = 0;
	int ammount = 0;
	for(int y = hmPos.y - (int)scanRange.y; y < hmPos.y + (int)scanRange.y; y++) {
		for(int x = hmPos.x - (int)scanRange.x; x < hmPos.x + (int)scanRange.x; x++) {
			ammount++;
			int readX = max(min(x, heightMap->getWidth() - 1), 0);
			int readY = max(min(y, heightMap->getHeight() - 1), 0);
			height += (*heightMap)[readX + readY * w];
		}
	}
	height /= (double)ammount;

	return height;
}

double World::getZ(ofVec2f pos) {
	double w = heightMap->getWidth();
	double h = heightMap->getHeight();
	double x1 = (int)pos.x;
	double x2 = x1 + 1;
	double y1 = (int)pos.y;
	double y2 = y1 + 1;

	//clip to heightMap size to avoid potential disaster
	x1 = fmax(0, fmin(x1, w - 1));
	x2 = fmax(0, fmin(x2, w - 1));
	y1 = fmax(0, fmin(y1, h - 1));
	y2 = fmax(0, fmin(y2, h - 1));

	//lookup height values
	double z11 = (*heightMap)[x1 + y1 * w];
	double z12 = (*heightMap)[x1 + y2 * w];
	double z21 = (*heightMap)[x2 + y1 * w];
	double z22 = (*heightMap)[x2 + y2 * w];

	//interpolate
	double r1 = (x2 - pos.x) * z11 + (pos.x - x1) * z21;
	double r2 = (x2 - pos.x) * z12 + (pos.x - x1) * z22;
	double z = (y2 - pos.y) * r1 + (pos.y - y1) * r2;
		
	return z;
}