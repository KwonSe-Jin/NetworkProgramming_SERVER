#pragma once

#include "Head.h"
#include "Nose.h"
#include "Body.h"
#include "Arm.h"
#include "Leg.h"
#include "Eyes.h"
#include "Ear.h"
#include "Beard.h"
#include "sword.h"
#include "Hero.h"

#include "Gun.h"
#include "particle.h"

extern Hero hero;

class Dog
{
	Head head;
	Nose nose;
	Body body;
	Arm armL;
	Arm armR;
	Leg legL;
	Leg legR;
	Eyes eyesL;
	Eyes eyesR;
	Ear earL;
	Ear earR;
	Beard beardvertical;
	Beard beardhorizon;
	Sword swordR;
	Sword swordL;

	int HP;
	int Attack;
	int Index;
	glm::vec3 Position;
	float Direction;

	float closelineX;
	float closelineZ;

public:
	Dog();
	~Dog();

	void draw();
	void update();

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	friend void AnimalCollideDog();
	friend void BulletCollideDog();
	friend void Dogroomtest();

	void initDog();
};

