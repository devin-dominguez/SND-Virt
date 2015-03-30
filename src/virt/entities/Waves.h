#pragma once

#include "ofMain.h"
#include "Entity.h"
#include "World.h"
#include "SoundObject.h"

class Settings;

class Wave : public Entity{
friend class Settings;
public:
	Wave(ofVec2f pos, double size, unsigned char color);
	~Wave();
	void update(double dt);
	void draw();

	static vector<Entity*> container;
	
	
protected:
	void grow(double dt);

	void kill();
	
	double growthRate;
	double initialGrowthRate;
	double decayRate;
	double timeRemaining;

	static double audioMessageInterval;
	static bool voices[12];
	unsigned int voiceNumber;

	SoundObject soundObject;

	
};

class EmitterWave : public Wave {
friend class Settings;
public:
	EmitterWave(ofVec2f pos, double size, unsigned char color);
private:
	static double initialGrowthRate;
	static double decayRate;
	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
};

class OriginWave : public Wave {
friend class Settings;
public:
	OriginWave(ofVec2f pos, double size);
private:
	static double initialGrowthRate;
	static double decayRate;
	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
};
