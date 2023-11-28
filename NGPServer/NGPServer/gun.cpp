#include "gun.h"



Gun::Gun(float posX, float posY, float posZ, float dirX, float dirY, float dirZ) :  PosX(posX), PosY(posY), PosZ(posZ),
dirX(dirX), dirY(dirY), dirZ(dirZ)
{
	startP = 0.8;

	Damage = 10;
}

Gun::~Gun()
{

}

void Gun::Update()
{
	//if (herodead) {
	//	Damage = 0;
	//}
	/*BulletCollideBear();
	BulletCollideDog();
	BulletCollideCat();*/
	startP += 0.3;
	//GunDir = glm::vec3(dirX * startP, dirY * startP, dirZ * startP) + glm::vec3(PosX, PosY, PosZ);
	
	/*glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.01, 0.01, 0.01));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(GunDir.x, GunDir.y, GunDir.z));
	Change = Trans * Scale;*/
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