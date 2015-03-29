#pragma once

#include "ofMain.h"
#include "Entity.h"

class Settings;

class SpecialFX : public Entity {
friend class Settings;
public:
	SpecialFX();
	void update(double dt);

	static vector<Entity*> container;
	static vector<Entity*> shockWaveContainer;

protected:
	double duration;
	double timeElapsed;
};
//--------------------------------------------------------------
//--------------------------------------------------------------
class ParticleTrailA : public SpecialFX {
friend class Settings;
public:
	ParticleTrailA(ofVec2f pos, double size);
	void draw();

private:
	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
	static unsigned char color;


};
//--------------------------------------------------------------
//--------------------------------------------------------------
class ParticleTrailB : public SpecialFX {
friend class Settings;
public:
	ParticleTrailB(ofVec2f startPos, ofVec2f endPos);
	void draw();

private:
	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
	static unsigned char color;

	ofVec2f endPos;

};
//--------------------------------------------------------------
//--------------------------------------------------------------
class Explosion : public SpecialFX {
public:
	Explosion(ofVec2f pos, double angle, double targetDistance, double minEase, double maxEase, double minDistance, double maxDistance);
	void update(double dt);
	static void Explode(unsigned char explosionType, ofVec2f pos, double targetDistance, unsigned int debrisNum, double minAngle = 0.0, double maxAngle = M_PI * 2.0);

protected:
	ofVec2f target;
	double easeRate;

};
//--------------------------------------------------------------
//--------------------------------------------------------------
class ExplosionA : public Explosion {
friend class Settings;
public:
	ExplosionA(ofVec2f pos, double angle, double targetDistance);
	void draw();

private:
	static double maxEase;
	static double minEase;
	static double minSize;
	static double maxSize;
	static double minDistance;
	static double maxDistance;

	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
	static unsigned char color;

	double angle;
	double length;

};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ExplosionB : public Explosion {
friend class Settings;
public:
	ExplosionB(ofVec2f pos, double angle, double targetDistance);
	void draw();

private:
	static double maxEase;
	static double minEase;
	static double minSize;
	static double maxSize;
	static double minDistance;
	static double maxDistance;

	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
	static unsigned char color;

};

class ExplosionP : public Explosion {
friend class Settings;
public:
	ExplosionP(ofVec2f pos, double angle, double targetDistance);
	void draw();

private:
	static double maxEase;
	static double minEase;
	static double minSize;
	static double maxSize;
	static double minDistance;
	static double maxDistance;

	static double duration;
	static double fadeInTime;
	static double fadeOutTime;
	static unsigned char color;
};
//--------------------------------------------------------------
//--------------------------------------------------------------
class ShockWave : public SpecialFX {
friend class Settings;
public:
	ShockWave(ofVec2f pos, double size);
	void update(double dt);
	void draw();
private:
	static double duration;
	static double fadeInTime;
	static double fadeOutTime;

	double initSize;
};