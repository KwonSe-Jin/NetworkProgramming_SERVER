#pragma once




class Animal
{
	float closelineX;
	float closelineZ;

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


	void initAnimals();
};

