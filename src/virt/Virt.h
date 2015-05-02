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

	struct emitterLocation {
		emitterLocation(ofVec2f pos, unsigned char color, Entity::Type type) :pos(pos), color(color), type(type){}
		ofVec2f pos;
		unsigned char color;
		Entity::Type type;
	};

	void updateEmitters(const vector<emitterLocation>& blobs);
private:

	double dt, pt;

	bool emitterPresentAtLocation(const emitterLocation& location);
	bool locationPresentAtEmitter(Entity* emitter, const vector<emitterLocation>& blobs);
	static double maxEmitterDistance;

};
