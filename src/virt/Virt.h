#pragma once

#include "ofMain.h"
#include "World.h"
#include "Settings.h"
#include "AudioSystem.h"
#include "entities/Entities.h"

class Virt {
public:
	void setup(ofPixels *heightMap);
	void update();
	void draw(int x, int y, double w, double h);

	struct emitterLocation {
		emitterLocation(ofVec2f pos, unsigned char color) :pos(pos), color(color) {}
		ofVec2f pos;
		unsigned char color;
	};

	void updateEmitters(const vector<emitterLocation>& blobs, string type);
private:

	double dt, pt;

};
