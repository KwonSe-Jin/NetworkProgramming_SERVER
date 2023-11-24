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

extern Hero hero[3];

class Cat
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
	Beard beardL1;
	Beard beardR1;
	Beard beardL2;
	Beard beardR2;
	Beard beardL3;
	Beard beardR3;
	Sword swordR;
	Sword swordL;
	float closelineX;
	float closelineZ;

	glm::vec3 Position;
	float Direction;
	bool CatCrushHero;

	int HP;
	int Attack;


public:
	Cat();
	~Cat();

	int Index = 0;
	void draw();
	void update();
	void damage();

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	friend void AnimalCollideCat();
	friend void BulletCollideCat();
	friend void CatAndRoomCollision();

	void initCat();
};

