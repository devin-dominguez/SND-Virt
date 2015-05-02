#pragma once

#include "ofMain.h"
#include "World.h"
#include "PhysicsObject.h"
#include "Emitters.h"
#include "SpecialFX.h"
#include "SoundObject.h"

class Settings;

class Particle : public PhysicsObject {
friend class Settings;
public:
	Particle(ofVec2f pos, unsigned char color, double angle, double initVelocity);
	void update(double dt);
	void collision(Entity* e);

	static vector<Entity*> container;

protected:
	static double minSpeed;
	static double maxDownTime;
	static double fadeInTime;
	static double fadeOutTime;


	double minSize;
	double maxSize;

	double downTime;

	bool hit;

	SoundObject soundObject;
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ParticleA : public Particle {
friend class Settings;
public:
	ParticleA(ofVec2f pos, unsigned char color, double angle);
	~ParticleA();
	void draw();
	void update(double dt);
	void collision(Entity* e);
	void kill();
protected:
	static double mass;
	static double minSize;
	static double maxSize;
	static double initVelocity;
	static double friction;
	static double pentrationFadeOutTime;
	static double trailInterval;
	static double explosionSize;
	static double penetrationArc;

	double timeRemaining;

	bool penetrating;
	double angleOfPenetration;

	static double audioMessageInterval;
	static bool voices[48];
	unsigned int voiceNumber;
};
//--------------------------------------------------------------
//--------------------------------------------------------------
class ParticleB : public Particle {
friend class Settings;
public:
	ParticleB(ofVec2f pos, unsigned char color, double angle);
	~ParticleB();
	void draw();
	void update(double dt);
	void kill();

protected:
	static double mass;
	static double minSize;
	static double maxSize;
	static double initVelocity;
	static double friction;
	static double explosionSize;



	ofVec2f lastPos;

	static double audioMessageInterval;
	static bool voices[12];
	unsigned int voiceNumber;

};

