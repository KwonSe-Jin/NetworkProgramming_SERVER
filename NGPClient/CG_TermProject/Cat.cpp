#include "Cat.h"

extern int CatCnt;

Cat::Cat() : 
head(Head(glm::vec3(207. / 255, 207. / 255, 207. / 255),1)),
nose(Nose(glm::vec3(36.  /255, 36.   / 255, 36.  / 255),1)), 
body(Body(glm::vec3(207. / 255, 207. / 255, 207. / 255),1)),
armL(Arm(glm::vec3(207. / 255, 207. / 255, 207. / 255), -1,1)),
armR(Arm(glm::vec3(207. / 255, 207. / 255, 207. / 255), 1,1)),
legL(Leg(glm::vec3(207. / 255, 207. / 255, 207. / 255), -1,1)),
legR(Leg(glm::vec3(207. / 255, 207. / 255, 207. / 255), 1,1)),
eyesL(Eyes(glm::vec3(255./255, 54. / 255, 54. / 255),-1,1)), 
eyesR(Eyes(glm::vec3(255. / 255, 54. / 255, 54. / 255), 1,1)), 
 Direction(0.f),
earL(Ear(glm::vec3(135. / 255, 135. / 255, 135. / 255), -1,1)),
earR(Ear(glm::vec3(135. / 255, 135. / 255, 135. / 255), 1,1)),
beardL1(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), -1, 1,1)),
beardR1(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), 1, 1,1)),
beardL2(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), -1, 2,1)),
beardR2(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), 1, 2,1)),
beardL3(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), -1, 3,1)),
beardR3(Beard(glm::vec3(36. / 255, 36. / 255, 36. / 255), 1, 3,1)),
swordL(Sword(glm::vec3(72. / 255, 255. / 255, 255. / 255), -1,1)),
swordR(Sword(glm::vec3(72. / 255, 255. / 255, 255. / 255), 1,1))
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urdX{ -104,-97  };
	uniform_real_distribution<float> urdZ{ -3, 3 };
	Position.x = urdX(dre);
	Position.y = -1.0f;
	Position.z = urdZ(dre);
	HP = 20;
	Attack = 10;
	CatCrushHero = false;
	++CatCnt;
	Index= CatCnt;

}


void Cat::draw()
{
	nose.draw();
	head.draw();
	armL.draw();
	armR.draw();
	body.draw();	
	legL.draw();
	legR.draw();
	eyesL.draw();
	eyesR.draw();
	earL.draw();
	earR.draw();

	beardL1.draw();
	beardR1.draw();
	beardL2.draw();
	beardR2.draw();
	beardL3.draw();
	beardR3.draw();
	swordL.draw();
	swordR.draw();
}


void Cat::update()
{


		//여기는 고양이 - 주인공 따라오기 알고리즘 
		//hero[].location();
		/*float dz = HeroLocationZ - Position.z;
		float dx = HeroLocationX - Position.x;

		Direction = atan2(dx, dz);

		closelineX = HeroLocationX - Position.x;
		closelineZ = HeroLocationZ - Position.z;

		if (catlive && !herodead) {

			if (!(closelineX <= 0.5 && closelineX >= -0.5)) {

				if (closelineX >= 0.5) {
					closelineX -= 0.01;
					Position.x += 0.01;
				}
				if (closelineX < -0.5) {
					closelineX += 0.01;
					Position.x -= 0.01;
				}
			}

			if (!(closelineZ <= 0.5 && closelineZ >= -0.5)) {
				if (closelineZ > 0.5) {
					closelineZ -= 0.01;
					Position.z += 0.01;
				}
				if (closelineZ < -0.5) {
					closelineZ += 0.01;
					Position.z -= 0.01;
				}

			}

		}

		if ((closelineX <= 0.5 && closelineX >= -0.5) && (closelineZ <= 0.5 && closelineZ >= -0.5)) {
			CatCrushHero = true;
			catattack[Index].Activate = true;
			++catattack[Index].AttackCount;
		}
		else {
			CatCrushHero = false;
			catattack[Index].Activate = false;
		}*/


		nose.keyIn(Position, Direction);
		head.keyIn(Position, Direction);
		armL.keyIn(Position, Direction);
		armR.keyIn(Position, Direction);
		body.keyIn(Position, Direction);
		legL.keyIn(Position, Direction);
		legR.keyIn(Position, Direction);
		eyesL.keyIn(Position, Direction);
		eyesR.keyIn(Position, Direction);
		earL.keyIn(Position, Direction);
		earR.keyIn(Position, Direction);
		beardL1.keyIn(Position, Direction);
		beardR1.keyIn(Position, Direction);
		beardL2.keyIn(Position, Direction);
		beardR2.keyIn(Position, Direction);
		beardL3.keyIn(Position, Direction);
		beardR3.keyIn(Position, Direction);
		swordL.keyIn(Position, Direction);
		swordR.keyIn(Position, Direction);


	}



Cat::~Cat()
{

}


void Cat::damage()
{
	HP -= 10;
	
}

void Cat::catinfo(SC_MONSTER_PACKET* p)
{
	Direction = p->direction;
	Position.x = p->x;
	Position.y = p->y;
	Position.z = p->z;
	HP = p->hp;
}

float Cat::getLeft()
{
	return Position.x - 0.12f;
}

float Cat::getRight()
{
	return Position.x + 0.12f;
}

float Cat::getBehind()
{
	return Position.z - 0.12f;
}

float Cat::getFront()
{
	return Position.z + 0.12f;
}

float Cat::getBottom()
{
	return Position.y;
}

float Cat::getTop()
{
	return 0.0f;				
}

void Cat::initCat()
{
	Direction=0.f;
	HP = 20;
	closelineX = 0;
	closelineZ = 0;
}
