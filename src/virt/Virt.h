#pragma once

#include "ofMain.h"
#include "World.h"
#include "Settings.h"
#include "AudioSystem.h"
#include "Entities.h"

class Virt {
public:
	void setup(ofPixels *heightMap);
	void update();
	void draw(int x, int y, double w, double h);
private:

	double dt, pt;

};
