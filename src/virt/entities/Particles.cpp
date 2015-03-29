#include "Particles.h"

double Particle::minSpeed = 8;
double Particle::maxDownTime = .2;
double Particle::fadeInTime = .125;
double Particle::fadeOutTime = .25;

vector<Entity*> Particle::container;

Particle::Particle(ofVec2f pos, unsigned char color, double angle, double initVelocity) {
	this->pos = pos;
	initPhysics();


	Entity::fadeInTime = fadeInTime;
	Entity::fadeOutTime = fadeOutTime;


	this->angle = angle;
	velocity.x = initVelocity * cos(angle);
	velocity.y = initVelocity * sin(angle);
	this->color = color;

	downTime = 0;

}
//--------------------------------------------------------------
//BASE PARTICLE
//--------------------------------------------------------------
void Particle::update(double dt) {
	switch(phase) {
		case START:
		collidable = false;
		if(!fadeIn(dt)) {
			phase = MIDDLE;
		}
		move(dt);
		break;

		case MIDDLE:
			collidable = true;
			if(velocity.length() <= minSpeed) {
				downTime += dt;
			}
			else {
				downTime = 0;
			}

			if(downTime > maxDownTime || pos.x + size > World::getSize().x ||
				pos.x - size < 0 || pos.y + size > World::getSize().y || pos.y -size < 0) {
				kill();
			}
			move(dt);
		break;

		case END:
			collidable = false;
			if(!fadeOut(dt)) {
				dead = true;
			}
		break;
	}
		size = (maxSize - minSize) * (z / 255.0) + minSize;

}

void Particle::collision(Entity* e) {
	if(e->getType() != SHOCKWAVE) {
		kill();
	}	
}

//--------------------------------------------------------------
//PARTICLE A
//--------------------------------------------------------------
double ParticleA::mass = 12;
double ParticleA::minSize = 10;
double ParticleA::maxSize = 20;
double ParticleA::initVelocity = 200;
double ParticleA::friction = .1;
double ParticleA::pentrationFadeOutTime = .1;
double ParticleA::trailInterval = .125;
double ParticleA::explosionSize = 2;
double ParticleA::penetrationArc = M_PI / 8.0;

ParticleA::ParticleA(ofVec2f pos, unsigned char color, double angle) : Particle(pos, color, angle, initVelocity) {
	PhysicsObject::mass = mass;
	PhysicsObject::friction = friction;

	Particle::minSize = minSize;
	Particle::maxSize = maxSize;

	type = PARTICLE_A;

	penetrating = false;

	timeRemaining = 0;
}
//--------------------------------------------------------------
void ParticleA::update(double dt) {
	Particle::update(dt);
	if(phase != END) {
		if(timeRemaining > 0) {
			timeRemaining -= dt;
		}
		else {
			timeRemaining = trailInterval;
			SpecialFX::container.push_back(new ParticleTrailA(pos, size * .25));
		}
	}

	if(penetrating) {
		move(dt);
	}
}

void ParticleA::kill() {
	Entity::kill();
	if(penetrating) { 
		Explosion::Explode('P', pos, velocity.length() / 3.0, 24, angleOfPenetration - penetrationArc, angleOfPenetration + penetrationArc);
		
	}
	else {
		Explosion::Explode('A', pos, size * explosionSize, 16);
	}
}
//--------------------------------------------------------------
void ParticleA::collision(Entity* e) {


	if(e->getType() == WAVE_EMITTER) {
		WaveEmitter* waveEmitter = dynamic_cast<WaveEmitter*>(e);
		if(waveEmitter->isWaveReady(color)) {

		penetrating = true;
		Entity::fadeInTime = pentrationFadeOutTime;
		ofVec2f dPos = pos - e->getPosition();
		angleOfPenetration = atan2(dPos.y, dPos.x);


		}
	}

	if(e->getType() == SHOCKWAVE) {
		ofVec2f dPos = pos - e->getPosition();
		double direction = atan2(dPos.y, dPos.x);
		double distance = pos.distance(e->getPosition());
		distance /= e->getSize();
		distance = 1.0 - distance;
		addForce(direction, 1000.0 * distance);

	}

	Particle::collision(e);

}
//--------------------------------------------------------------
void ParticleA::draw() {
	ofPushMatrix();
	ofPushStyle();
		ofTranslate(pos);
		ofRotate(angle * 180 / M_PI);
		ofSetColor(colorTable[color], alpha);
		ofBeginShape();
			ofVertex(size, 0);
			ofVertex(-size, size / 2.0);
			ofVertex(0, 0);
			ofVertex(-size, -size / 2.0);
			ofVertex(size, 0);
		ofEndShape();
	ofPopStyle();
	ofPopMatrix();
}
//-------------------------------------------------------------- 
//PARTICLE B
//--------------------------------------------------------------
double ParticleB::mass = 16.0;
double ParticleB::minSize = 10.0;
double ParticleB::maxSize = 25.0;
double ParticleB::initVelocity = 280.0;
double ParticleB::friction = .125;
double ParticleB::explosionSize = 2.5;

ParticleB::ParticleB(ofVec2f pos, unsigned char color, double angle) : Particle(pos, color, angle, initVelocity)
{
	PhysicsObject::mass = mass;
	PhysicsObject::friction = friction;

	Particle::minSize = minSize;
	Particle::maxSize = maxSize;

	type = PARTICLE_B;

	lastPos = pos;
}

void ParticleB::update(double dt) {
	Particle::update(dt);

		if(phase == MIDDLE) {
	
				SpecialFX::container.push_back(new ParticleTrailB(pos, lastPos));
				lastPos = pos;
			
		}
}

void ParticleB::kill() {
	Entity::kill();
	Explosion::Explode('B', pos, size * explosionSize, 32);
	SpecialFX::shockWaveContainer.push_back(new ShockWave(pos, size * explosionSize * 2));
}

//--------------------------------------------------------------
void ParticleB::draw() {
	ofPushMatrix();
	ofPushStyle();
		ofTranslate(pos);
		ofRotate(angle * 180 / M_PI);
		ofSetColor(colorTable[color], alpha);
		
		ofSetColor(colorTable[color], alpha);

		ofSetRectMode(OF_RECTMODE_CENTER);

		ofRectRounded(origin, size * 2, size * 2 * .5, 10);
		ofTriangle(0, -.5 * size, -size, -size, -.5 * size, -.5 * size);
		ofTriangle(0, .5 * size, -size, size, -.5 * size, .5 * size);
	ofPopStyle();
	ofPopMatrix();


}