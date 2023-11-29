#include "Bear.h"

extern bool BearLife;
Bear::Bear() :
	head(Head(glm::vec3(255. / 255, 167. / 255, 167. / 255), 3)),
	body(Body(glm::vec3(255. / 255, 167. / 255, 167. / 255), 3)),
	bodyf(Body(glm::vec3(255. / 255, 239. / 255, 239. / 255), 4)),
	armL(Arm(glm::vec3(255. / 255, 167. / 255, 167. / 255), -1, 3)),
	armR(Arm(glm::vec3(255. / 255, 167. / 255, 167. / 255), 1, 3)),
	legL(Leg(glm::vec3(255. / 255, 167. / 255, 167. / 255), -1, 3)),
	legR(Leg(glm::vec3(255. / 255, 167. / 255, 167. / 255), 1, 3)),
	earL(Ear(glm::vec3(255. / 255, 167. / 255, 167. / 255), -1, 3)),
	earR(Ear(glm::vec3(255. / 255, 167. / 255, 167. / 255), 1, 3)),

	eyebrowL(Beard(glm::vec3(0. / 255, 0. / 255, 0. / 255), -1, 1, 3)),
	eyebrowR(Beard(glm::vec3(0. / 255, 0. / 255, 0. / 255), 1, 2, 3)),

	nose(Nose(glm::vec3(36. / 255, 36. / 255, 36. / 255), 3)),
	eyesL(Eyes(glm::vec3(255. / 255, 54. / 255, 54. / 255), -1, 3)),
	eyesR(Eyes(glm::vec3(255. / 255, 54. / 255, 54. / 255), 1, 3)),
	Position(glm::vec3(0.f, -1.f, -100.f)), Direction(0.f),
	swordL(Sword(glm::vec3(72. / 255, 255. / 255, 255. / 255), -1, 3)),
	swordR(Sword(glm::vec3(72. / 255, 255. / 255, 255. / 255), 1, 3))
{
	HP = 100;
	Attack = 30;
}


void Bear::draw()
{
	nose.draw();
	head.draw();
	armL.draw();
	armR.draw();
	body.draw();
	bodyf.draw();
	legL.draw();
	legR.draw();
	eyesL.draw();
	eyesR.draw();
	earL.draw();
	earR.draw();
	eyebrowL.draw();
	eyebrowR.draw();

	swordL.draw();
	swordR.draw();
}

void Bear::update()
{
	//hero.location();
	//float dz = HeroLocationZ - Position.z;
	//float dx = HeroLocationX - Position.x;

	//Direction = atan2(dx, dz);

	//closelineX = HeroLocationX - Position.x;
	//closelineZ = HeroLocationZ - Position.z;

	//if (bearlive && !herodead) {
	//	if (!(closelineX <= 0.5 && closelineX >= -0.5)) {

	//		if (closelineX >= 0.5) {
	//			closelineX -= 0.03;
	//			Position.x += 0.03;
	//		}
	//		if (closelineX < -0.5) {
	//			closelineX += 0.03;
	//			Position.x -= 0.03;
	//		}
	//	}
	//	if (!(closelineZ <= 0.5 && closelineZ >= -0.5)) {
	//		if (closelineZ > 0.5) {
	//			closelineZ -= 0.03;
	//			Position.z += 0.03;
	//		}
	//		if (closelineZ < -0.5) {
	//			closelineZ += 0.03;
	//			Position.z -= 0.03;
	//		}
	//	}
	//}


	//if ((closelineX <= 0.5 && closelineX >= -0.5) && (closelineZ <= 0.5 && closelineZ >= -0.5)) {
	//	bearattack.Activate = true;
	//	++bearattack.AttackCount;
	//}
	//else {
	//	bearattack.Activate = false;
	//}

	nose.keyIn(Position, Direction);
	head.keyIn(Position, Direction);
	armL.keyIn(Position, Direction);
	armR.keyIn(Position, Direction);
	body.keyIn(Position, Direction);
	bodyf.keyIn(Position, Direction);
	legL.keyIn(Position, Direction);
	legR.keyIn(Position, Direction);
	eyesL.keyIn(Position, Direction);
	eyesR.keyIn(Position, Direction);
	earL.keyIn(Position, Direction);
	earR.keyIn(Position, Direction);
	eyebrowL.keyIn(Position, Direction);
	eyebrowR.keyIn(Position, Direction);

	swordL.keyIn(Position, Direction);
	swordR.keyIn(Position, Direction);
}

void Bear::bearinfo(SC_MONSTER_PACKET* p)
{
	Direction = p->direction;
	Position.x = p->x;
	Position.y = p->y;
	Position.z = p->z;
	HP = p->hp;
}




Bear::~Bear()
{
}

float Bear::getLeft()
{
	return Position.x - 0.14f;
}

float Bear::getRight()
{
	return Position.x + 0.14f;
}

float Bear::getBehind()
{
	return Position.z - 0.14f;
}

float Bear::getFront()
{
	return Position.z + 0.14f;
}

float Bear::getBottom()
{
	return Position.y;
}

float Bear::getTop()
{
	return 0.0f;				//계산하기 귀찮으니 대충 0으로 잡겠음(의미없는값)
}

void Bear::InitBear()
{
	HP = 100;
	Attack = 30;
	Position = glm::vec3(0.f, -1.f, -100.f);
	Direction= 0.f;
	closelineX = 0;
	closelineZ = 0;
}


