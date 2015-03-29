#pragma once

#include "ofMain.h"
#include "World.h"
#include "Entity.h"

class PhysicsObject : public Entity {
public:
	void initPhysics();
	void addForce(ofVec2f force);
	void addForce(double direction, double magnitude);

protected:
	ofVec2f velocity;
	double mass;
	double z;
	double angle;
	double friction;
	ofVec2f mapLocation;

	//Physics and Motion Stuff
	double getIncline(double direction, double distance);
	vector<ofVec2f> forces;
	void addCurveForces();
	void integrateForces(double dt);
	void move(double dt);
};