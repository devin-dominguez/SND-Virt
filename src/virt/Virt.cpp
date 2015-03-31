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

	Emitter::container.push_back(new WaveEmitter(ofVec2f(100, 100), 4));
	Emitter::container.push_back(new WaveEmitter(ofVec2f(600, 300), 4));
	Emitter::container.push_back(new ParticleEmitter(ofVec2f(400, 120), 2));
	Emitter::container.push_back(new ParticleEmitter(ofVec2f(500, 450), 1));
	Emitter::container.push_back(new ParticleEmitter(ofVec2f(200, 350), 2));


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
		//Entity::drawAll(SpecialFX::shockWaveContainer);
		Entity::drawAll(SpecialFX::container);
		Entity::drawAll(Particle::container);
		Entity::drawAll(Wave::container);
		Entity::drawAll(Emitter::container);
		Entity::drawAll(Origin::container);

	ofPopMatrix();
}

void Virt::updateEmitters(const vector<emitterLocation>& blobs, string type) {

}
