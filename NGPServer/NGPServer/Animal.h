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
	bool AnimalCrushHero;

	int Attack;




public:
	float Direction;
	int HP;
	int AnimalType;
	int Index = 0;
	float PosX = 0;
	float PosY = 0;
	float PosZ = 0;
	Animal(int type, int id);
	~Animal();

	void update();
	void damage();
	float calculateDistance(float targetX, float targetZ);;
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	//friend void AnimalCollideCat();
	//friend void BulletCollideCat();
	//friend void CatAndRoomCollision();

	void initAnimals();
};

