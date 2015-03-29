#include "Entity.h"
unsigned long Entity::currentId = 0;
//--------------------------------------------------------------
Entity::Entity() {
	dead = false;
	collidable = false;
	phase = START;
	alpha = 0;
	id = currentId++;;
}
//--------------------------------------------------------------
//empty virtual functions
Entity::~Entity() {}
void Entity::update(double dt) {}
void Entity::draw() {}
void Entity::collision(Entity* e) {}
//--------------------------------------------------------------
bool Entity::fadeIn(double dt) {
	if(alpha < 255) {
		alpha += (255.0 / fadeInTime) * dt;
		alpha = fmin(255.0, alpha);
		return true;
	}
	else return false;
}
//--------------------------------------------------------------
bool Entity::fadeOut(double dt) {
	if(alpha > 0) {
		alpha -= (255.0 / fadeOutTime) * dt;
		alpha = fmax(0.0, alpha);
		return true;
	}
	else return false;
}
//--------------------------------------------------------------
void Entity::kill() {
	phase = END;
}

//--------------------------------------------------------------
//getters
ofVec2f Entity::getPosition() {return pos;}
unsigned char Entity::getColor() {return color;}
double Entity::getSize() {return size;}
bool Entity::isDead() {return dead;}
bool Entity::isCollidable() {return collidable;}
Entity::Type Entity::getType() {return type;}
unsigned long Entity::getId() {return id;}

//STATIC METHODS + DATA
//--------------------------------------------------------------
void Entity::updateAll(vector<Entity*> &e, double dt) {
	int s = e.size();
	for(int i = 0; i < s; i++) {
		e[i]->update(dt);
	}
}

//--------------------------------------------------------------
void Entity::drawAll(vector<Entity*> &e) {
	int s = e.size();
	for(int i = 0; i < s; i++) {
		e[i]->draw();
	}
}

//--------------------------------------------------------------
void Entity::collideAll(vector<Entity*> &e1, vector<Entity*> &e2) {
	int s1 = e1.size();
	int s2 = e2.size();
	for(int i = 0; i < s1; i++) {
		for(int j = 0; j < s2; j++) {
			if(e1[i]->isCollidable() && e2[j]->isCollidable() && areColliding(e1[i], e2[j])) {
				e1[i]->collision(e2[j]);
				e2[j]->collision(e1[i]);
			}
		}
	}
}
//--------------------------------------------------------------
void Entity::pruneVector(vector<Entity*> &e) {
	int i = e.size();
	while(i--) {
		if(e[i]->dead) {
			delete e[i];
			e.erase(e.begin() + i);
		}
	}
}
//--------------------------------------------------------------
bool Entity::areColliding(Entity *e1, Entity *e2) {
	float a = e1->size - e2->size;
	a *= a;
	
	float b1 = e1->pos.x - e2->pos.x;
	b1 *= b1;

	float b2 = e1->pos.y - e2->pos.y;
	b2 *= b2;

	float b = b1 + b2;
	
	float c = e1->size + e2->size;
	c *= c;

	return a <= b && b <= c;
}

//--------------------------------------------------------------
ofFloatColor Entity::colorTable[5] = {
	ofFloatColor(1.0),
	ofFloatColor(1.0, 1.0, 0.0),
	ofFloatColor(0.0, 1.0, 1.0),
	ofFloatColor(1.0, 0.0, 1.0),
	colorTable[1] * colorTable[2]
};

ofVec2f Entity::origin = ofVec2f(0, 0);