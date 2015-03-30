#pragma once

#include "ofMain.h"
#include "World.h"
#include "Entity.h"
#include "Particles.h"
#include "Waves.h"
//#include "SoundObject.h"

class Settings;

class Emitter : public Entity {
friend class Settings;

public:
	Emitter(ofVec2f pos);
	~Emitter();
	void update(double dt);
	void collision(Entity* e);

	static vector<Entity*> container;
protected:
	static double fadeInTime;
	static double fadeOutTime;
	static double snapOutRate;
	static double snapInRate;

	//collision realted things
	bool beingHitByWave[5];
	bool hasBeenHitByWave[5];
	//wobble
	double hitAngle;
	double amp, ampTarget;
	ofVec2f localPos;
	double displacementAmmount;
	double wobbleAmmount;
	double wobbleFreq;
	double t;

	double height;

	static double audioMessageInterval; 	
	static bool voices[8];
	unsigned int voiceNumber;
	SoundObject soundObject;

};
//--------------------------------------------------------------
//--------------------------------------------------------------
class WaveEmitter : public Emitter {
friend class Settings;

public:
	WaveEmitter(ofVec2f pos);

	void update(double dt);
	void draw();
	void collision(Entity* e);
	bool isWaveReady(unsigned char waveColor);

private:
	static double size;
	static double coolDownTime;
	static double ringSize;
	static double wobbleAmmount;
	static double wobbleFreq;
	static double displacementAmmount;


	double coolDown[5];
	double spinnerAmp[5];
	bool waveReady[5];



	//spinners
	static double spinnerMinRate;
	static double spinnerMaxRate;
	static double spinnerEase;
	static double spinnerOffset;
	static unsigned int spinnerResolution;

	double spinnerRotation;
	vector<unsigned char> spinnerColors;

	void spawnWave(unsigned char color);
	vector<unsigned char> waveQueue;

};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ParticleEmitter : public Emitter {
friend class Settings;
public:
	ParticleEmitter(ofVec2f pos, unsigned char color);

	void update(double dt);
	void draw();
	void collision(Entity *e);

private:
	static double size;
	static double ringSize;
	static double displacementAmmount;
	static double wobbleAmmount;
	static double wobbleFreq;

	static unsigned int particleAAmmount;
	static unsigned int particleBAmmount;
	static double minRate;
	static double maxRate;
	static double coolDownTime;
	static double easeRate;
	double rotation;

	double coolDownA, coolDownB;
	bool readyA, readyB;
	double ampA, ampB;
	void spawnParticles(char particleType);
	vector<char> particleQueue;

	vector<unsigned char> waveColors;
};
