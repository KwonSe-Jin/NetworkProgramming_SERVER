#include "gun.h"
#include <iostream>
using namespace std;


Gun::Gun(float posX, float posY, float posZ, float dirX, float dirY, float dirZ) :  PosX(posX), PosY(posY), PosZ(posZ),
dirX(dirX), dirY(dirY), dirZ(dirZ)
{
	startP = 0.8;

	Damage = 10;
	GunDirX = 0;
	GunDirY = 0;
	GunDirZ = 0;
	status = true;
}

Gun::~Gun()
{

}

void Gun::Update()
{
	startP += 0.3;
	GunDirX = dirX * startP + PosX;
	GunDirY = dirY * startP + PosY;
	GunDirZ = dirZ * startP + PosZ;	

}


float Gun::GetPosZ() {
	return GunDirZ;
}

float Gun::GetPosX() {
	return GunDirX;
}


float Gun::getLeft()
{
	return GunDirX - 0.03f;
}

float Gun::getRight()
{
	return GunDirX + 0.03f;
}

float Gun::getBehind()
{
	return GunDirZ - 0.03f;
}

float Gun::getFront()
{
	return GunDirZ + 0.03f;
}

float Gun::getBottom()
{
	return GunDirY;
}

float Gun::getTop()
{
	return 0.0f;
}