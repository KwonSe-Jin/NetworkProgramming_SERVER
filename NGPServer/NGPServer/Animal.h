#pragma once
//#include "Hero.h"
//#include "Gun.h"
//#include "particle.h"

//extern Hero hero;



class Animal
{
	float closelineX;
	float closelineZ;

	//glm::vec3 Position;
	float Direction;
	bool AnimalCrushHero;

	int HP;
	int Attack;
	int Index = 0;


	int AnimalType;


public:
	float PosX = 0;
	float PosY = 0;
	float PosZ = 0;
	Animal(int type, int id);
	~Animal();

	void update();
	void damage();

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	//friend void AnimalCollideCat();
	//friend void BulletCollideCat();
	//friend void CatAndRoomCollision();

	void initCat();
};

