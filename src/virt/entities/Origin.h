#pragma once

#include "ofMain.h"
#include "Entity.h"
#include "World.h"
#include "Waves.h"
#include "SoundObject.h"

class Settings;

class Origin : public Entity {
friend class Settings;
public:
	Origin();

	void update(double dt);
	void draw();

	static vector<Entity*> container;
	

private:
	static double size;
	static double ringSize;
	static double audioMessageInterval;

	//audio stuff
	SoundObject soundObject;

	//trigger parameters
	double timeElapsed;
	static double minInterval, maxInterval;


	void trigger();

	//wobble parameters
	double t;
	static double criticalRange;
	ofVec2f wobble; 
	static ofVec2f freqMax, ampMax;
	ofVec2f freq, amp;
};