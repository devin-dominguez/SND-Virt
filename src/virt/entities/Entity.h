#pragma once

#include "ofMain.h"

class Entity {
public:
	Entity();
	virtual ~Entity();
 
	enum Type {
		ORIGIN,
		PARTICLE_A,
		PARTICLE_B,
		WAVE,
		WAVE_EMITTER,
		PARTICLE_EMITTER,
		SHOCKWAVE
	};

	//getters
	ofVec2f getPosition();
	unsigned char getColor();
	double getSize();
	Type getType();
	unsigned long getId();



	bool isDead();
	bool isCollidable();

	//individual methods
	virtual void update(double dt);
	virtual void draw();
	virtual void collision(Entity* e);
	virtual void kill();

	//iterator methods
	static void updateAll(vector<Entity*> &e, double dt);
	static void drawAll(vector<Entity*> &e);
	static void pruneVector(vector<Entity*> &e);

	static void collideAll(vector<Entity*> &e1, vector<Entity*> &e2);
	
	//other static methods
	static bool areColliding(Entity* e1, Entity* e2);

protected:


	//instance id number
	static unsigned long currentId;
	unsigned long id;

	//display + collision attributes
	ofVec2f pos;
	unsigned char color;
	double alpha;
	double size;
	bool dead;
	bool collidable;

	//other attributes
	Type type;

	enum {
		START,
		MIDDLE,
		END
	} phase;



	//fade animations
	bool fadeIn(double dt);
	bool fadeOut(double dt);
	double fadeInTime, fadeOutTime;

	//color lookup table
	static ofFloatColor colorTable[];

	//origin vector
	static ofVec2f origin;
};
