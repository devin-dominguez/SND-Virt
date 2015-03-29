#include "Origin.h"

double Origin::size = 20;
double Origin::ringSize = 4;
double Origin::minInterval = 4;
double Origin::maxInterval = 12;
double Origin::criticalRange = .25;
ofVec2f Origin::freqMax = ofVec2f(64, 79);
ofVec2f Origin::ampMax = ofVec2f(2, 3);

double Origin::audioMessageInterval = 0.1;

vector<Entity*> Origin::container;

//--------------------------------------------------------------
Origin::Origin() {
	type = ORIGIN;
	Entity::size = size;
	color = 0;
	timeElapsed = 0;
	pos = World::getSize() * .5;

	soundObject.setup("Origin", 0, audioMessageInterval);
	soundObject.messageNow("x", pos.x / World::getSize().x);
	soundObject.messageNow("y", pos.y / World::getSize().y);
	soundObject.messageNow("time", 0);
	soundObject.messageNow("amp", 0);
}
//--------------------------------------------------------------
void Origin::update(double dt) {

	double interval = (1.0 - World::getAverageHeight(pos, size) / 255.0 ) * (maxInterval - minInterval) + minInterval;
	

	//countdown to trigger
	t = timeElapsed / interval;

	if(timeElapsed < interval) {
		timeElapsed += dt;
		timeElapsed = fmin(interval, timeElapsed);
	}
	else {
		timeElapsed = 0;
		trigger();
	}

	//wobble
	if(t > criticalRange) {
		double localT = (t - criticalRange) / (1 - criticalRange);
		localT *= localT;

		freq = localT * freqMax;
		amp = localT * ampMax;

	}
	else {
		amp.x = fmax(0.0, amp.x - 3.0 * dt);
		amp.y = fmax(0.0, amp.y - 3.0 * dt);
	}

	wobble.x = amp.x * cos(t * M_PI * 2 * freq.x);
	wobble.y = amp.y * sin(t * M_PI * 2 * freq.y);

	soundObject.messageLater("time", timeElapsed / interval);
	soundObject.messageLater("amp", amp.x / ampMax.x);
	soundObject.update(dt);
}
//--------------------------------------------------------------
void Origin::trigger() {
	Wave::container.push_back(new OriginWave(pos, size));
}
//--------------------------------------------------------------
void Origin::draw() {
	ofPushMatrix();
	ofPushStyle();
		ofEnableAlphaBlending();
		ofTranslate(pos);
		ofSetLineWidth(1);
		ofFill();
		
		//outer ring
		ofSetColor(colorTable[color]);
		ofCircle(wobble, size);
		
		//masking
		ofSetColor(0);
		ofCircle(wobble, size -ringSize);

		//inner ring
		ofNoFill();
		ofSetColor(colorTable[color]);
		double innerSize = 1.0 - (1.0 - t) * (1.0 - t);
		ofCircle(wobble, size * innerSize);

	ofPopStyle();
	ofPopMatrix();

}