#include "Gun.h"
#include <algorithm>

Gun::Gun(float x, float y, float z) : Unit(1.f), PosX(x), PosY(y), PosZ(z)
{

	startP = 0.8;

	color = glm::vec3(255 / 255., 221 / 255., 115 / 255.);

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
	//BulletCollideBear();
	//BulletCollideDog();
	//BulletCollideCat();
	/*startP += 0.3;*/
	/*GunDir = glm::vec3(dirX * startP, dirY * startP, dirZ * startP) + glm::vec3(PosX, PosY, PosZ);*/
	
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.05, 0.05, 0.05));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(PosX, PosY, PosZ));
	Change = Trans * Scale;
}


float Gun::GetPosZ() {
	return GunDir.z;
}

float Gun::GetPosX() {
	return GunDir.x;
}
void Gun::Draw()
{

	glBindVertexArray(sphereVAO);
	GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	GLuint aColor = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(aColor, color.r, color.g, color.b, 1.0);
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);
}

void Gun::setInfo(SC_BULLET_PACKET* p)
{
	PosX = p->dirx;
	PosY = p->diry;
	PosZ = p->dirz;
	status = p->status;
	/*GunDir.x = p->dirx;
	GunDir.z = p->diry;
	GunDir.y = p->dirz;*/

}


float Gun::getLeft()
{
	return GunDir.x - 0.03f;
}

float Gun::getRight()
{
	return GunDir.x + 0.03f;
}

float Gun::getBehind()
{
	return GunDir.z - 0.03f;
}

float Gun::getFront()
{
	return GunDir.z + 0.03f;
}

float Gun::getBottom()
{
	return GunDir.y;
}

float Gun::getTop()
{
	return 0.0f;
}