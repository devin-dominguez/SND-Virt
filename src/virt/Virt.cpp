#include "Virt.h"

void Virt::setup(ofPixels *heightMap) {
	ofSetCircleResolution(180);
	ofEnableSmoothing();

	World::setup(heightMap);

	dt = 0;
	pt = ofGetElapsedTimef();

	Settings::setup();
	AudioSystem::setup("localhost", 6666);

	Origin::container.push_back(new Origin());

	Emitter::container.push_back(new WaveEmitter(ofVec2f(200, 100), 4));
//	Emitter::container.push_back(new WaveEmitter(ofVec2f(600, 300), 4));
//	Emitter::container.push_back(new ParticleEmitter(ofVec2f(400, 120), 2));
//	Emitter::container.push_back(new ParticleEmitter(ofVec2f(500, 450), 1));
//	Emitter::container.push_back(new ParticleEmitter(ofVec2f(200, 350), 2));


}
//--------------------------------------------------------------
void Virt::update() {
	dt = ofGetElapsedTimef() - pt;
	dt = fmin(dt, .1);
	//dt *= .5;
	pt = ofGetElapsedTimef();

	AudioSystem::update();

	Entity::updateAll(Origin::container, dt);
	Entity::updateAll(Wave::container, dt);
	Entity::updateAll(Emitter::container, dt);
	Entity::updateAll(Particle::container, dt);
	Entity::updateAll(SpecialFX::container, dt);
	Entity::updateAll(SpecialFX::shockWaveContainer, dt);

	Entity::collideAll(Emitter::container, Wave::container);
	Entity::collideAll(Emitter::container, Particle::container);
	Entity::collideAll(Particle::container, SpecialFX::shockWaveContainer);

	Entity::pruneVector(Wave::container);
	Entity::pruneVector(Emitter::container);
	Entity::pruneVector(Particle::container);
	Entity::pruneVector(SpecialFX::container);
	Entity::pruneVector(SpecialFX::shockWaveContainer);

}
//--------------------------------------------------------------
void Virt::draw(int x, int y, double w, double h) {
	ofPushMatrix();
	ofTranslate(x, y);
		double scaleX = w / World::getSize().x;
		double scaleY = h / World::getSize().y;
		ofScale(scaleX, scaleY);

		//uncomment this to see the range of the shockwave effect
		Entity::drawAll(SpecialFX::shockWaveContainer);
		Entity::drawAll(SpecialFX::container);
		Entity::drawAll(Particle::container);
		Entity::drawAll(Wave::container);
		Entity::drawAll(Emitter::container);
		Entity::drawAll(Origin::container);

	ofPopMatrix();
}


double Virt::maxEmitterDistance = 10.0;

void Virt::updateEmitters(const vector<emitterLocation>& blobs) {
	//add emitters to locations with blob
	for(unsigned int i = 0; i < blobs.size(); i++) {
		if(!emitterPresentAtLocation(blobs[i])) {
			if(blobs[i].type == Entity::WAVE_EMITTER) {
				Emitter::container.push_back(new WaveEmitter(blobs[i].pos, blobs[i].color));
				cout << "Wave Emitter created at " << blobs[i].pos << endl;
			}
			else if(blobs[i].type == Entity::PARTICLE_EMITTER) {
				Emitter::container.push_back(new ParticleEmitter(blobs[i].pos, blobs[i].color));
				cout << "Particle Emitter created at " << blobs[i].pos << endl;
			}
		}
	}
	//remove emitters from locations without blobs
	for(unsigned int i = 0; i < Emitter::container.size(); i++) {
		if(!locationPresentAtEmitter(Emitter::container[i], blobs)) {
			Emitter::container[i]->kill();
			cout << "Emitter removed from " << Emitter::container[i]->getPosition() << endl;
		}
	}
}


bool Virt::emitterPresentAtLocation(const emitterLocation& location) {

	for(unsigned int i = 0; i < Emitter::container.size(); i++) {
		if(location.type == Emitter::container[i]->getType() &&
				location.color == Emitter::container[i]->getColor() &&
				location.pos.distance(Emitter::container[i]->getPosition()) <= maxEmitterDistance) {
			return true;
		}
	}
	return false;
}

bool Virt::locationPresentAtEmitter(Entity* emitter, const vector<emitterLocation>& blobs) {
	for(unsigned int i = 0; i < blobs.size(); i++) {
		if(emitter->getType() == blobs[i].type &&
				emitter->getColor() == blobs[i].color &&
				emitter->getPosition().distance(blobs[i].pos) <= maxEmitterDistance) {
			return true;
		}
	}
	return false;
}
