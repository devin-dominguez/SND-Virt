#include "Emitters.h"

//--------------------------------------------------------------
//BASE EMITTER
//--------------------------------------------------------------
double Emitter::fadeInTime = 1;
double Emitter::fadeOutTime = 1;
double Emitter::snapOutRate = 10;
double Emitter::snapInRate = 10;

double Emitter::audioMessageInterval = .1;
bool Emitter::voices[] = {false};

vector<Entity*> Emitter::container;

//--------------------------------------------------------------
Emitter::Emitter(ofVec2f pos){
	localPos = pos;
	Entity::pos = localPos;

	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;

	amp = 0;
	ampTarget = 0;
	hitAngle = 0;
	t = 0;

	int i = 0;
	while(voices[i]) {
		i++;
	}
	voiceNumber = i;
	voices[voiceNumber] = true;

	soundObject.setup("Emitter", voiceNumber, audioMessageInterval);

	soundObject.messageNow("color", color);
	soundObject.messageNow("x", pos.x / World::getSize().x);
	soundObject.messageNow("y", pos.y / World::getSize().y);
	soundObject.messageNow("phase", phase);
	soundObject.messageNow("hit", 0);
}
//--------------------------------------------------------------
Emitter::~Emitter() {
	voices[voiceNumber] = false;
}
//--------------------------------------------------------------
void Emitter::update(double dt) {
	//phase related stuff
	ofVec2f displacement, wobble;
	t += dt;
	double wobbleAmp;
	bool notHit = false;
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
			//displace when hit by origin
			double ease;
			if(beingHitByWave[0]) {
				ampTarget = 1;
				ease = snapOutRate;
			}
			else {
				ampTarget = 0;
				ease = snapInRate;
			}

			amp = ofLerp(amp, ampTarget, ease * dt);
			displacement = amp * displacementAmmount * ofVec2f(cos(hitAngle), sin(hitAngle));
			wobbleAmp = sin(t * M_PI * 2 * wobbleFreq);
			wobble = amp * wobbleAmp * wobbleAmmount * ofVec2f(cos(hitAngle - M_PI_2), sin(hitAngle - M_PI_2));
			pos = localPos + displacement + wobble;

			//reset collision
			for(int i = 0; i < 5; i++) {
				
				
				if(!beingHitByWave[i] && hasBeenHitByWave[i]) {
					notHit = true;
				}

				hasBeenHitByWave[i] = beingHitByWave[i];

				beingHitByWave[i] = false;
			}

			if(notHit) {
				soundObject.messageNow("hit", 0);
			}


		break;

		case END:
			collidable = false;
			if(!fadeOut(dt)) {
				dead = true;
			}
		break;
	}

	height = World::getAverageHeight(pos, size) / 255.0;
	

	soundObject.update(dt);

}
//--------------------------------------------------------------

void Emitter::kill() {
	Entity::kill();
	soundObject.messageNow("phase", phase);
}
//--------------------------------------------------------------
void Emitter::collision(Entity* e) {
	if(e->getType() == WAVE) {
		unsigned char waveColor = e->getColor();
	
		beingHitByWave[waveColor] = true;
		if(waveColor == 0) {
			hitAngle = atan2(pos.y - e->getPosition().y, pos.x - e->getPosition().x);
		}
		bool newHit = true;	
		for(int i = 0; i < 5; i++) {
			if(hasBeenHitByWave[i]) {
				newHit = false;
			}
		}
		if(newHit) {
			soundObject.messageNow("hit", 1);
		}

	}
}
//--------------------------------------------------------------
//WAVE EMITTER
//--------------------------------------------------------------
double WaveEmitter::size = 70.0;
double WaveEmitter::coolDownTime = 6.0;
double WaveEmitter::ringSize = 8.0;
double WaveEmitter::wobbleAmmount = 1.0;
double WaveEmitter::wobbleFreq = 3.0;
double WaveEmitter::spinnerEase = 5;
double WaveEmitter::spinnerMaxRate = .15;
double WaveEmitter::spinnerMinRate = .05;
double WaveEmitter::spinnerOffset = 20.0;
double WaveEmitter::displacementAmmount = 3.0;
unsigned int WaveEmitter::spinnerResolution = 6;
//--------------------------------------------------------------
WaveEmitter::WaveEmitter(ofVec2f pos) : Emitter(pos) {
	type = WAVE_EMITTER;
	Entity::size = size;
	Emitter::displacementAmmount = displacementAmmount;
	Emitter::wobbleAmmount = wobbleAmmount;
	Emitter::wobbleFreq = wobbleFreq;

	for(int i = 0; i < 5; i++) {
		coolDown[i] = 0;
		spinnerAmp[i] = 0;
		waveReady[i] = false;
	}

	spinnerRotation = 0;
	spinnerColors.push_back(1);
	spinnerColors.push_back(2);

	//spinnerColors.push_back(3);

}
//--------------------------------------------------------------
void WaveEmitter::update(double dt) {

	//spawn queued waves
	for(unsigned int i = 0; i < waveQueue.size(); i++) {
		spawnWave(waveQueue[i]);
	}
	waveQueue.clear();
	if(phase == MIDDLE) {
		for(int i = 0; i < 5; i++) {
			if(coolDown[i] > 0) {
				coolDown[i] -= dt;
				coolDown[i] = fmax(0, coolDown[i]);
				waveReady[i] = false;
			}
			else {
				waveReady[i] = true;
			}
		}
	}

	//generic emitter stuff
	Emitter::update(dt);

	//spinner rotational stuff
	double rate = height * (spinnerMaxRate - spinnerMinRate) + spinnerMinRate;
	spinnerRotation += rate * dt * 360.0;

for(unsigned int i = 0; i < spinnerColors.size(); i++) {
			double* spinAmp = &spinnerAmp[spinnerColors[i]];
			double amp = waveReady[spinnerColors[i]] ? spinnerOffset : -ringSize * 2;
			*spinAmp = ofLerp(*spinAmp, amp, spinnerEase * dt);
		}

}
//--------------------------------------------------------------
void WaveEmitter::collision(Entity* e) {
	Emitter::collision(e);
	int particleColor = e->getColor();
	if(e->getType() == PARTICLE_A && waveReady[particleColor]) {
			waveQueue.push_back(particleColor);
			coolDown[particleColor] = coolDownTime;
	}
}
//--------------------------------------------------------------
bool WaveEmitter::isWaveReady(unsigned char waveColor) {
	return waveReady[waveColor];
}

//--------------------------------------------------------------
void WaveEmitter::spawnWave(unsigned char color) {
	Wave::container.push_back(new EmitterWave(pos, size, color));
}
//--------------------------------------------------------------
void WaveEmitter::draw() {
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(pos);

		//spinners			
		ofPushStyle();
		ofSetCircleResolution(spinnerResolution);
		ofNoFill();
		for(unsigned int i = 0; i < spinnerColors.size(); i++) {
			double angle = 360.0 / (double)spinnerResolution * double(i) / (double)spinnerColors.size();
			ofPushMatrix();
			ofRotate(angle + spinnerRotation);
				ofSetColor(colorTable[spinnerColors[i]],alpha);
				ofCircle(origin, size +  spinnerAmp[spinnerColors[i]]);
			ofPopMatrix();
		}
		ofPopStyle();
		
		//main ring
		ofSetColor(colorTable[4], alpha);
		ofCircle(origin, size);
		//masking
		ofSetColor(0, alpha);
		ofCircle(origin, size - ringSize);
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
//PARTICLE EMITTER
//--------------------------------------------------------------
double ParticleEmitter::size = 48;
double ParticleEmitter::ringSize = 4;
double ParticleEmitter::displacementAmmount = 5;
double ParticleEmitter::wobbleAmmount = .5;
double ParticleEmitter::wobbleFreq = 5;
unsigned int ParticleEmitter::particleAAmmount = 9;
unsigned int ParticleEmitter::particleBAmmount = 3;
double ParticleEmitter::minRate = -.1;
double ParticleEmitter::maxRate = .1;
double ParticleEmitter::coolDownTime = 3;
double ParticleEmitter::easeRate = 10;

ParticleEmitter::ParticleEmitter(ofVec2f pos, unsigned char color) : Emitter(pos) {
	type = PARTICLE_EMITTER;
	Entity::size = size;
	Emitter::displacementAmmount = displacementAmmount;
	Emitter::wobbleAmmount = wobbleAmmount;
	Emitter::wobbleFreq = wobbleFreq;

	this->color = color;
	coolDownA = coolDownTime;
	coolDownB = coolDownTime;

	readyA = false;
	readyB = false;

	rotation = 0;
	ampA = 0;
	ampB = 0;
}
//--------------------------------------------------------------
void ParticleEmitter::update(double dt) {
	//spawn queued particles
	for(unsigned int i = 0; i < particleQueue.size(); i++) {
		spawnParticles(particleQueue[i]);
	}
	particleQueue.clear();
	//generic emitter stuff
	Emitter::update(dt);
	if(phase == MIDDLE) {
		if(coolDownA > 0) {
			coolDownA -= dt;
			coolDownA = fmax(0, coolDownA);
			readyA = false;
		}
		else {
			readyA = true;
		}
		if(coolDownB > 0) {
			coolDownB -= dt;
			coolDownB = fmax(0, coolDownB);
			readyB = false;
		}
		else {
			readyB = true;
		}
	}


	double rate = height * (maxRate - minRate) + minRate;
	rotation += rate * dt * 360.0;

	double ampTargetA = size * (readyA ? 0.5 : .08);
	ampA = ofLerp(ampA, ampTargetA, easeRate * dt);

	double ampTargetB = size * (readyB ?  0.5 :  0.125);
	ampB = ofLerp(ampB, ampTargetB, easeRate * dt);

	waveColors.clear();
}
//--------------------------------------------------------------
void ParticleEmitter::collision(Entity *e) {
	Emitter::collision(e);
	switch(e->getType()) {
		case WAVE:
			if(beingHitByWave[0] && !hasBeenHitByWave[0] && readyA) {
				particleQueue.push_back('A');
				coolDownA = coolDownTime;
			}
			else if(beingHitByWave[color] && !hasBeenHitByWave[color] && readyB) {
				particleQueue.push_back('B');
				coolDownB = coolDownTime;
			}
		break;
		default:
		break;
	}

}

//--------------------------------------------------------------
void ParticleEmitter::spawnParticles(char particleType) {
	unsigned int ammount = 0;
	if(particleType == 'A') {
		ammount = particleAAmmount;
	}
	else if(particleType =='B') {
		ammount = particleBAmmount;
	}

	for(unsigned int i = 0; i < ammount; i++) {
		double angle = M_PI * 2 * (double)i / (double)ammount + rotation;
		ofVec2f offset(cos(angle), sin(angle));
		offset *= size;

		if(particleType == 'A') {
			Particle::container.push_back(new ParticleA(pos + offset, color, angle));
		}
		else if(particleType =='B') {
			Particle::container.push_back(new ParticleB(pos + offset, color, angle));
		}

	}
}

//--------------------------------------------------------------
void ParticleEmitter::draw() {
	ofPushMatrix();
	ofPushStyle();
		ofTranslate(pos);
		ofSetColor(colorTable[color], alpha);
		ofFill();
		//particle guides

		//A guides
		for(unsigned int i = 0; i < particleAAmmount; i++) {
			double angle = 360.0 * (double)i / (double)particleAAmmount + rotation;
			ofPushMatrix();
				ofRotate(angle);
				ofBeginShape();
					ofVertex(size + ampA, 0),
					ofVertex(size - ringSize, -3.0);
					ofVertex(size - ringSize, 3.0);
				ofEndShape();
			ofPopMatrix();
		}
		//B guides
		for(unsigned int i = 0; i < particleBAmmount; i++) {
			double angle = 360.0 * (double)i / (double)particleBAmmount + rotation ;
			ofPushMatrix();
				ofRotate(angle);
					ofSetRectMode(OF_RECTMODE_CENTER);
					ofRectRounded(size, 0, ampB, size * .35, 10);
				ofPopMatrix();
		}

		//main ring
		ofSetColor(colorTable[color], alpha);
		ofCircle(origin, size);
		//masking
		ofSetColor(0, alpha);
		ofCircle(origin, size - ringSize);
		
	ofPopStyle();
	ofPopMatrix();
}
