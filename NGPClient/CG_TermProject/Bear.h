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

extern Hero hero[3];

class Bear
{
	Head head;
	Nose nose;
	Body body;
	Body bodyf;
	Arm armL;
	Arm armR;
	Leg legL;
	Leg legR;
	Eyes eyesR;
	Eyes eyesL;
	Ear earL;
	Ear earR;
	Sword swordR;
	Sword swordL;
	Beard eyebrowL;
	Beard eyebrowR;
	float closelineX;
	float closelineZ;

	glm::vec3 Position;
	float Direction;
	int Attack;


public:
	Bear();
	~Bear();
	int HP;

	void draw();
	void update();
	void bearinfo(SC_MONSTER_PACKET* p);
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();

	friend void BulletCollideBear();
	friend void Bearroomtest();

	void InitBear();
};

