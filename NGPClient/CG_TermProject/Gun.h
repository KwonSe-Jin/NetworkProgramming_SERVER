#pragma once
#include "Header.h"

class Gun
{
	const glm::mat4 Unit;

	glm::mat4 Change;

	glm::vec3 color;

	float PosX;
	float PosY;
	float PosZ;
	float dirX;
	float dirY;
	float dirZ;
	int  Damage;
	glm::vec3 GunDir;
	float startP;
	

public:
	Gun(float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
	~Gun();
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	void Update();
	float GetPosZ();
	float GetPosX();
	void Draw();
	friend void BulletCollideCat();
	friend void BulletCollideDog();
	friend void BulletCollideBear();

	void Loction();
};