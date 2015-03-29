#include "AutoHeightMap.h"

void AutoHeightMap::setup(int width, int height) {
	heightMap.allocate(width, height, OF_IMAGE_GRAYSCALE);
	scaleX = .025;
	scaleY = .025;
	z = 0;
}

void AutoHeightMap::update() {
	for(int x = 0; x < heightMap.getWidth(); x++) {
		for(int y = 0; y < heightMap.getHeight(); y++) {
			char zValue = ofNoise(x * scaleX, y * scaleY ) * 128;

			heightMap.getPixelsRef()[y * heightMap.getWidth() + x] = zValue;

		}
	}
	heightMap.reloadTexture();
}

void AutoHeightMap::draw(int x, int y, int width, int height) {

	heightMap.draw(x, y, width, height);
}

ofPixels* AutoHeightMap::getPixelsRef() {
	return &heightMap.getPixelsRef();
}
