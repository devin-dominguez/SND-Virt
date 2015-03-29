#include "Waves.h"

double Wave::audioMessageInterval = .1;
bool Wave::voices[12] = {false};

vector<Entity*> Wave::container;



//--------------------------------------------------------------
Wave::Wave(ofVec2f pos, double size, unsigned char color) {
	type = WAVE;
	this->pos = pos;
	this->size = size;
	this->color = color;

	int i = 0;
	while(voices[i]) {
		i++;
	}
	voiceNumber = i;
	voices[voiceNumber] = true;

	soundObject.setup("Wave", voiceNumber, audioMessageInterval);

	soundObject.messageNow("color", color);
	soundObject.messageNow("x", pos.x / World::getSize().x);
	soundObject.messageNow("y", pos.y / World::getSize().y);
	soundObject.messageNow("phase", phase);
	soundObject.messageNow("size", size / World::getSize().x);
}
//--------------------------------------------------------------
Wave::~Wave() {
	voices[voiceNumber] = false;
}
//--------------------------------------------------------------
void Wave::update(double dt) {
	switch(phase) {
		case START:
			collidable = false;
			if(!fadeIn(dt)) {
				phase = MIDDLE;
				soundObject.messageNow("phase", phase);
			}
		break;

		case MIDDLE:
		collidable = true;
		if(timeRemaining > 0) {
			timeRemaining -= dt;
		}
		else {
			kill();
		}
		break;

		case END:
		collidable = true;
			if(!fadeOut(dt)) {
				dead = true;
			}
		break;
	}

	grow(dt);
	soundObject.messageLater("size", size / World::getSize().x);
	soundObject.update(dt);
}
//--------------------------------------------------------------
void Wave::kill() {
	Entity::kill();
	soundObject.messageNow("phase", phase);
}
//--------------------------------------------------------------
void Wave::grow(double dt) {
	growthRate = fmax(0, growthRate - decayRate * dt);
	size += growthRate * dt;
}
//--------------------------------------------------------------
void Wave::draw() {
	ofPushMatrix();
	ofPushStyle();
		ofEnableAlphaBlending();
		ofTranslate(pos);
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(colorTable[color], alpha);
		ofCircle(0, 0, size);


	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
double EmitterWave::initialGrowthRate = 55;
double EmitterWave::decayRate = 5;
double EmitterWave::duration = 5;
double EmitterWave::fadeInTime = .25;
double EmitterWave::fadeOutTime = 1;
//--------------------------------------------------------------
EmitterWave::EmitterWave(ofVec2f pos, double size, unsigned char color) : Wave(pos, size, color) {
	
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;

	Wave::initialGrowthRate = initialGrowthRate;
	Wave::growthRate = initialGrowthRate;
	Wave::decayRate = decayRate;
	
	timeRemaining = duration;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
double OriginWave::initialGrowthRate = 140;
double OriginWave::decayRate = 20;
double OriginWave::duration = 5;
double OriginWave::fadeInTime = .1;
double OriginWave::fadeOutTime = 1;
//--------------------------------------------------------------
OriginWave::OriginWave(ofVec2f pos, double size) : Wave(pos, size, 0) {
	
	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;

	Wave::initialGrowthRate = initialGrowthRate;
	Wave::growthRate = initialGrowthRate;
	Wave::decayRate = decayRate;
	
	timeRemaining = duration;
}