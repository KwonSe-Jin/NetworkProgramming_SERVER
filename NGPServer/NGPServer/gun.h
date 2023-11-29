#pragma once

class Gun
{

	
	float dirX;
	float dirY;
	float dirZ;

	
	float startP;
	

public:
	Gun(float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
	~Gun();
	int  Damage;
	bool status;
	float PosX;
	float PosY;
	float PosZ;
	float GunDirX;
	float GunDirY;
	float GunDirZ;
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
	void Update();
	float GetPosZ();
	float GetPosX();
	//friend void BulletCollideCat();
	//friend void BulletCollideDog();
	//friend void BulletCollideBear();

};