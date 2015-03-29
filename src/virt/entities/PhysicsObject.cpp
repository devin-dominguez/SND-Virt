#include "PhysicsObject.h"

void PhysicsObject::initPhysics() {
	mapLocation = World::getMapPos(pos);
}

//--------------------------------------------------------------
double PhysicsObject::getIncline(double direction, double distance) {
	ofVec2f XY(cos(direction), sin(direction));
	double front  = World::getZ(mapLocation + XY * distance);
	double back = World::getZ(mapLocation - XY * distance);
	double incline = atan2(front - back, 2 * distance / World::getWorldToMapRatio().x);

	return incline;
}
//--------------------------------------------------------------
void PhysicsObject::addForce(ofVec2f force) {
	forces.push_back(force);
}
//--------------------------------------------------------------
void PhysicsObject::addForce(double direction, double magnitude) {
	ofVec2f XY(cos(direction), sin(direction));
	ofVec2f force = magnitude * XY;
	addForce(force);
}
//--------------------------------------------------------------
void PhysicsObject::addCurveForces() {
	double weight = mass * World::getGravity();
	double inclineX = getIncline(0, 2);
	double inclineY = getIncline(M_PI_2, 2);
	
	double fGravityX = weight *  sin(inclineX);
	double fGravityY = weight * sin(inclineY);
	
	double fNormalX = weight * cos(inclineX);
	double fNormalY = weight * cos(inclineY);
	
	double fFrictionX = fNormalX * World::getFriction() * friction;
	fFrictionX = fabs(fFrictionX) * ((velocity.x < 0) ? 1 : -1);
	
	double fFrictionY = fNormalY * World::getFriction() * friction;
	fFrictionY = fabs(fFrictionY) * ((velocity.y < 0) ? 1 : -1);
	
	double fNetX = cos(inclineX) * (fGravityX + fFrictionX);
	double fNetY = cos(inclineY) * (fGravityY + fFrictionY);

	addForce(ofVec2f(fNetX, fNetY));

}
//--------------------------------------------------------------
void PhysicsObject::integrateForces(double dt) {
	//Euler integration
	for(unsigned int i = 0; i < forces.size(); i++) {
		velocity += (forces[i] / mass) * dt;
	}

	forces.clear();
}
//--------------------------------------------------------------
void PhysicsObject::move(double dt) {
	addCurveForces();
	integrateForces(dt);
	
	angle = atan2(velocity.y, velocity.x);
	double incline = getIncline(angle, 1);
	pos += cos(incline) * velocity * dt;
	mapLocation = World::getMapPos(pos);
	z = World::getZ(mapLocation);
}
