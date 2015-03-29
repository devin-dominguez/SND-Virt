#include "SpecialFX.h"

vector<Entity*> SpecialFX::container;
vector<Entity*> SpecialFX::shockWaveContainer;

SpecialFX::SpecialFX() {
	timeElapsed = 0.0;

}
//--------------------------------------------------------------
void SpecialFX::update(double dt) {
	switch(phase) {
		case START:
			if(!fadeIn(dt)) {
				phase = MIDDLE;
			}
		break;

		case MIDDLE:
			if(timeElapsed < duration) {
				timeElapsed += dt;
			}
			else {
				kill();
			}
		break;

		case END:
			if(!fadeOut(dt)) {
				dead = true;
			}
		break;
	}
}


//--------------------------------------------------------------
//PARTICLE TRAIL A
//--------------------------------------------------------------
double ParticleTrailA::fadeInTime = .1;
double ParticleTrailA::duration = 1.0;
double ParticleTrailA::fadeOutTime = 0;
unsigned char ParticleTrailA::color = 0;

ParticleTrailA::ParticleTrailA(ofVec2f pos, double size) {
	this->pos = pos;
	this->size = size;

	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	collidable = false;
}
//--------------------------------------------------------------
void ParticleTrailA::draw() {
	ofPushStyle();
		ofNoFill();
		ofSetColor(colorTable[color], alpha * .75);
		double drawScale = 1.0 - timeElapsed / duration;
		drawScale = fmax(fmin(drawScale, 1.0), 0.0);
		ofCircle(pos, size * drawScale);
	ofPopStyle();
}

//--------------------------------------------------------------
//PARTICLE TRAIL B
//--------------------------------------------------------------
double ParticleTrailB::fadeInTime = 0.1;
double ParticleTrailB::duration = 1.25;
double ParticleTrailB::fadeOutTime = 0.125;
unsigned char ParticleTrailB::color = 0;


ParticleTrailB::ParticleTrailB(ofVec2f startPos, ofVec2f endPos) {
	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	Entity::color = color;

	pos = startPos;
	this->endPos = endPos;
	collidable = false;
}
//--------------------------------------------------------------
void ParticleTrailB::draw() {
	ofPushStyle();
		ofSetColor(colorTable[color], alpha * .75);
		ofLine(pos, endPos);
	ofPopStyle();
}


//--------------------------------------------------------------
//EXPLOSION
//--------------------------------------------------------------
Explosion::Explosion(ofVec2f pos, double angle, double targetDistance, double minEase, double maxEase, double minDistance, double maxDistance) {
	this->pos = pos;
	easeRate = ofRandom(minEase, maxEase);
	double distance = ofRandom(minDistance, maxDistance) * targetDistance;
	target = distance * ofVec2f(cos(angle), sin(angle));
	target += pos;
	
	collidable = false;
}
//--------------------------------------------------------------
void Explosion::update(double dt) {
	SpecialFX::update(dt);
	pos.x = ofLerp(pos.x, target.x, easeRate * dt);
	pos.y = ofLerp(pos.y, target.y, easeRate * dt);
}
//--------------------------------------------------------------
void Explosion::Explode(unsigned char explosionType, ofVec2f pos, double targetDistance, unsigned int debrisNum, double minAngle, double maxAngle) {
	for(unsigned int i = 0; i < debrisNum; i++) {
		double direction = ofRandom(minAngle, maxAngle);
		if(explosionType == 'A') {
			container.push_back(new ExplosionA(pos, direction, targetDistance));
		}

		if(explosionType == 'B') {
			container.push_back(new ExplosionB(pos, direction, targetDistance));
		}

		if(explosionType == 'P') {
			container.push_back(new ExplosionP(pos, direction, targetDistance));
		}
	}
}


//--------------------------------------------------------------
//EXPLOSION A
//--------------------------------------------------------------
double ExplosionA::fadeInTime = .025;
double ExplosionA::duration = 0.5;
double ExplosionA::fadeOutTime = .25;
unsigned char ExplosionA::color = 0;
double ExplosionA::minDistance = .5;
double ExplosionA::maxDistance = 1.0;


double ExplosionA::minEase = 5;
double ExplosionA::maxEase = 20;

//--------------------------------------------------------------
ExplosionA::ExplosionA(ofVec2f pos, double angle, double targetDistance) : Explosion(pos, angle, targetDistance, minEase, maxEase, minDistance, maxDistance) {
	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	Entity::color = color;
	length = ofRandom(0.0, .25) * targetDistance;

	this->angle = angle;

}
//--------------------------------------------------------------
void ExplosionA::draw() {
	ofPushMatrix();
	ofPushStyle();

		ofTranslate(pos);
		ofRotate(angle * 180.0 / M_PI);
		
		ofSetColor(colorTable[color], alpha);
		ofLine(0, 0, length, 0);
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
//EXPLOSION B
//--------------------------------------------------------------
double ExplosionB::fadeInTime = .1;
double ExplosionB::duration = 0.5;
double ExplosionB::fadeOutTime = 1.25;
unsigned char ExplosionB::color = 0;

double ExplosionB::minEase = 5;
double ExplosionB::maxEase = 20;
double ExplosionB::minSize = .5;
double ExplosionB::maxSize = 2;
double ExplosionB::minDistance = .25;
double ExplosionB::maxDistance = 1.0;
//--------------------------------------------------------------
ExplosionB::ExplosionB(ofVec2f pos, double angle, double targetDistance) : Explosion(pos, angle, targetDistance, minEase, maxEase, minDistance, maxDistance) {
	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	Entity::color = color;

	size = ofRandom(minSize, maxSize);

}
//--------------------------------------------------------------
void ExplosionB::draw() {
	ofPushStyle();
		ofSetColor(colorTable[color], alpha);
		ofCircle(pos, size);
	ofPopStyle();
}

//--------------------------------------------------------------
//EXPLOSION P
//--------------------------------------------------------------
double ExplosionP::fadeInTime = .1;
double ExplosionP::duration = 1;
double ExplosionP::fadeOutTime = 1.25;
unsigned char ExplosionP::color = 4;

double ExplosionP::minEase = 1;
double ExplosionP::maxEase = 10;
double ExplosionP::minSize = .25;
double ExplosionP::maxSize = 5;
double ExplosionP::minDistance = 0.0;
double ExplosionP::maxDistance = 1.0;
//--------------------------------------------------------------
ExplosionP::ExplosionP(ofVec2f pos, double angle, double targetDistance) : Explosion(pos, angle, targetDistance, minEase, maxEase, minDistance, maxDistance) {
	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	Entity::color = color;

	size = ofRandom(minSize, maxSize);

}
//--------------------------------------------------------------
void ExplosionP::draw() {
	ofPushStyle();
		ofSetColor(colorTable[color], alpha);
		ofCircle(pos, size);
	ofPopStyle();
}


//--------------------------------------------------------------
//SHOCKWAVE
//--------------------------------------------------------------
double ShockWave::duration = .25;
double ShockWave::fadeInTime = 0;
double ShockWave::fadeOutTime = 0;

ShockWave::ShockWave(ofVec2f pos, double size) {
	SpecialFX::duration = duration;
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;
	this->pos = pos;
	initSize = size;
	this->size = 0;

	type = SHOCKWAVE;
	collidable = true;

}
//--------------------------------------------------------------
void ShockWave::update(double dt) {
	SpecialFX::update(dt);

	size = initSize * timeElapsed / duration;
}
//--------------------------------------------------------------
void ShockWave::draw() {
	ofPushStyle();
		ofNoFill();
		ofCircle(pos, size);
	ofPopStyle();
}