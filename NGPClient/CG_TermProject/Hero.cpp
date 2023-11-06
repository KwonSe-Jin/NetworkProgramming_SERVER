#include "Hero.h"

#include "Sound.h"
extern Sound playSound;

extern float HeroMovY;
extern float MovX;
extern float MovZ;
extern bool catlive;
extern bool doglive ;
extern bool bearlive;
extern bool herodead;
extern bool firstmap;
extern float HeroLocationX;
extern float HeroLocationZ;
extern CatAttack catattack[AnimCnt];
extern DogAttack dogattack[AnimCnt];
extern BearAttack bearattack;
extern bool jumpUp;

Hero::Hero() : Unit(1.f)
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd{ 0, 255 };
	color = glm::vec3(255 / 255., 255 / 255., 255 / 255.);
	scaleX = 0.3;
	scaleY = 0.3;
	scaleZ = 0.3;
	PosX = 0;
	PosY = 0.5;
	PosZ = 10.0;
	HP = 100;

}

Hero::~Hero()
{
	
}

void Hero::damage()
{

	HP = 100;
	lightColorG = 1.0;
	lightColorB = 1.0;
	for (int i = 0; i < 6; ++i) {
		HP -= (catattack[i].AttackCount) * 10 / 24;
		lightColorG -= catattack[i].AttackCount * 0.1f/24.;
		lightColorB -= catattack[i].AttackCount * 0.1f/24.;

		HP -= (dogattack[i].AttackCount) * 20 / 24;
		lightColorG -= dogattack[i].AttackCount * 0.2f / 24.;
		lightColorB -= dogattack[i].AttackCount * 0.2f / 24.;
	}


	HP -= (bearattack.AttackCount) * 30 / 24;
	lightColorG -= bearattack.AttackCount * 0.3f / 24.;
	lightColorB -= bearattack.AttackCount * 0.3f / 24.;


	if (lightColorG < 0.1) {
		herodead = true;
		lightColorB = 0.1;
		lightColorG = 0.1;
		lightColorR = 0.1;

		// 죽으면 사운드 멈춤
		playSound.stopBGM();
	}
}



int Hero::InfoHP() {
	return HP/10;
}

void Hero::Update()
{ 

	damage();
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(scaleX, scaleY, scaleZ));
	glm::mat4 Trans;
	
		Trans = glm::translate(Unit, glm::vec3(PosX, PosY + HeroMovY, PosZ ));


		if (PosX < -1 && firstmap)
		{
			PosX += 0.1;
		}
		if (PosX > 1 && firstmap)
		{
			PosX -= 0.1;
		}

		if (PosZ > 11 && firstmap)
		{
			PosZ -= 0.1;
		}

		if (PosZ < -10 && catdead ==0) {
			PosX = -100;
			PosZ = 0;
			catlive = true;
			bearlive = false;
			doglive = false;

			CarX = -100;
			CarY = 1.0;
			CarZ = 0.0;

			CarDX = -100;
			CarDY = 0.0;
			CarDZ = 0.0;
			ortho = 5.0;

			lightPosX = -100;
			lightPosY = 15.0;
			lightPosZ = 0;

			// 캣라운드로 들어갈 때 브금 실행
			playSound.BasicBGM();
			firstmap = false;
		}
		if (PosZ < -7 && catdead == 6) {
			PosX = 100;
			PosZ = 0;
			doglive = true;
			catlive = false;
			bearlive = false;
			CarX = 100;
			CarY = 1.0;
			CarZ = 0.0;

			CarDX = 100;
			CarDY = 0.0;
			CarDZ = 0.0;
			ortho = 5.0;

			lightPosX = 100;
			lightPosY = 15.0;
			lightPosZ = 0;

			catdead++;
		}
		if (PosZ < -7 && dogdead == 6) {
			PosX = 0;
			PosZ = -100;
			dogdead++;

			doglive = false;
			bearlive = true;
			catlive = false;
			CarX = 0.0;
			CarY = 1.0;
			CarZ = -100.0;

			CarDX = 0.0;
			CarDY = 0.0;
			CarDZ = -100.0;
			ortho = 5.0;

			lightPosX = 0;
			lightPosY = 15.0;
			lightPosZ = -100;
		}
		
	glm::mat4 AddTrans = glm::translate(Unit, glm::vec3(0., 1., 0.));
	Change = Trans * Scale * AddTrans;
	if (catlive  )
	{
		HeroVSCat();
	}
		
	if(doglive)
		HeroVSDog();
		
	if(bearlive)
		HeroVSBear();
}

void Hero::Jump()
{
	if (isJump) {
		if (jumpUp) {
			HeroMovY += 0.1f;
			cameraJump +=0.1f;
			if (HeroMovY >= 1) {
				jumpUp = false;
			}
		}
		else {
			HeroMovY -= 0.1f;
			cameraJump -= 0.1f;
			if (HeroMovY <= 0.f) {
				isJump = false;
			}
		}
	}
	
	
}

void Hero::Draw()
{

	glBindVertexArray(VAO);
	GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	GLuint aColor = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(aColor, color.r, color.g, color.b,1.0);
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));
	glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);


}

void Hero::location()
{
	HeroLocationX = PosX;
	HeroLocationZ = PosZ;
}

void Hero::ISW()
{
	carAddZ -= 0.075 * glm::cos(glm::radians(VAngleY));
	carAddX += 0.075 * glm::sin(glm::radians(VAngleY));
	PosZ -= 0.075 * glm::cos(glm::radians(VAngleY));
	PosX += 0.075 * glm::sin(glm::radians(VAngleY));
}

void Hero::ISA()
{
	carAddZ -= 0.075 * glm::sin(glm::radians(VAngleY));
	carAddX -= 0.075 * glm::cos(glm::radians(VAngleY));
	PosZ -= 0.075 * glm::sin(glm::radians(VAngleY));
	PosX -= 0.075 * glm::cos(glm::radians(VAngleY));

}

void Hero::ISS()
{
	carAddZ += 0.075 * glm::cos(glm::radians(VAngleY));
	carAddX -= 0.075 * glm::sin(glm::radians(VAngleY));
	PosZ += 0.075 * glm::cos(glm::radians(VAngleY));
	PosX -= 0.075 * glm::sin(glm::radians(VAngleY));
}

void Hero::ISD()
{
	carAddZ += 0.075 * glm::sin(glm::radians(VAngleY));
	carAddX += 0.075 * glm::cos(glm::radians(VAngleY));
	PosZ  += 0.075 * glm::sin(glm::radians(VAngleY));
	PosX  += 0.075 * glm::cos(glm::radians(VAngleY));
}

float Hero::getLeft()
{
	return PosX - 0.12f;
}

float Hero::getRight()
{
	return PosX + 0.12f;
}

float Hero::getBehind()
{
	return PosZ - 0.12f;
}

float Hero::getFront()
{
	return PosZ + 0.12f;
}






///////////////////camera part///////////////////


void Hero::camera()
{

	cameraPos = glm::vec3(carAddX + 0, carAddY + cameraJump, carAddZ);
	if (carAddX < -1 && firstmap)
	{
		carAddX += 0.1;
	}
	if (carAddX > 1 && firstmap)
	{
		carAddX -= 0.1;
	}
	if (carAddZ > 11 && firstmap)
	{
		carAddZ -= 0.1;
	}
	if (bearlive) {
		if (third) {
			carAddX = 0;
			carAddZ = -100;
			third = false;
		}
		if (-5 > carAddX)
			carAddX += 1.0;
		if (5 < carAddX)
			carAddX -= 1.0;
		if (-105 > carAddZ)
			carAddZ += 1.0;
		if (-95 < carAddZ)
			carAddZ -= 1.0;
		//bearlive = false;
	}

	else if (doglive) {
		if (second) {
			carAddX = 100;
			carAddZ = 0;
			second = false;
		}

		if (dogdead == 6 && carAddX < 101 && carAddX > 98 && carAddZ < -4.5 && isW) {
			carAddZ -= 0.1;

		}
		else {
			if (95 > carAddX)
				carAddX += 1.0;
			if (105 < carAddX)
				carAddX -= 1.0;
			if (-5 > carAddZ)
				carAddZ += 1.0;
			if (+5 < carAddZ)
				carAddZ -= 1.0;
		}

	}
	else if (catlive) {
		if (first) {
			carAddX = -100;
			carAddZ = 0;
			first = false;
		}

		if (catdead == 6 && carAddX > -101 && carAddX < -98 && carAddZ < -4.5 && isW) {
			carAddZ -= 0.1;

		}
		else {
			if (-105 > carAddX)
				carAddX += 1.0;
			if (-95 < carAddX)
				carAddX -= 1.0;
			if (-5 > carAddZ)
				carAddZ += 1.0;
			if (+5 < carAddZ)
				carAddZ -= 1.0;
		}

	}

	glm::mat4 VAngleY_Rot = glm::rotate(glm::mat4(1.0f), glm::radians(-VAngleY), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 VAngleX_Rot = glm::rotate(glm::mat4(1.0f), glm::radians(-VAngleX), glm::vec3(1.0, 0.0, 0.0));

	glm::vec3 CDir(0.f, 0.f, -10.f);
	CDir = VAngleY_Rot * VAngleX_Rot * glm::vec4(CDir, 1.f);
	CDir.x += 1.f;

	glm::vec3 cameraDirection(cameraPos + CDir);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraDirection.y -= 0.5f;
	cameraPos.y -= 0.5f;
	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	TermGunDir = normalize(cameraDirection - cameraPos);

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

}

void Hero::TopView()
{
	glm::vec3 cameraPos = glm::vec3(CarX, CarY, CarZ);
	glm::vec3 cameraDirection = glm::vec3(CarDX, CarDY, CarDZ);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 view = glm::mat4(1.0f);


	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

void Hero::camera3D()
{
	glm::vec3 cameraPos = glm::vec3(0.f, 5.f, 5.f);
	glm::vec3 cameraDirection = glm::vec3(0.f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);


	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

}

void Hero::cameraProjection()
{
	projection = glm::perspective(glm::radians(90.0f), (float)width / height, 0.1f, 200.0f);
	projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glViewport(0, 0, width, height);
}

void Hero::DrawHP()
{
	for (int i = 0; i <HP/10; ++i) {
		glm::mat4 Change;
		Change = Unit;
		GLuint projectionTransform = glGetUniformLocation(shaderID, "projectionTransform");
		glUniformMatrix4fv(projectionTransform, 1, GL_FALSE, glm::value_ptr(Change));
		Change = Unit;
		GLuint viewTransform = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(viewTransform, 1, GL_FALSE, glm::value_ptr(Change));

		glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.3));
		glm::mat4 Trans = glm::translate(Unit, glm::vec3(-0.95, 0.95, 0));
		Change = Trans * Scale;
		Change = glm::translate(Unit, glm::vec3(i * 0.08f, 0, 0)) * Change;
		GLuint modelTransform = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(Change));

		glDrawArrays(GL_POLYGON, 0, 4);
	}
}

void Hero::cameraProjection2()
{
	glViewport(width / 1.26, height / 1.35, 200, 200);
	projection = glm::mat4(1.0f);
	projection = glm::ortho(-ortho, ortho, -ortho, ortho, -ortho, ortho);
	projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void Hero::VAngleMotion(int x, int y)
{
	VAngleY += (x - width / 2) / 12.5;


	VAngleX += (y - height / 2) / 12.5;
	if (VAngleX >= 15.f) {
		VAngleX = 15.f;
	}
	else if (VAngleX <= -15.f) {
		VAngleX = -15.f;
	}
	glutWarpPointer(width / 2, height / 2);
}

void Hero::initHero()
{
	scaleX = 0.3;
	scaleY = 0.3;
	scaleZ = 0.3;
	PosX = 0;
	PosY = 0.5;
	PosZ = 10.0;
	HP = 100;

	//카메라 초기화~
	first = true;
	second = true;
	third = true;

	 carAddX = 0;
	 carAddY = 0;
	 carAddZ=10;
	 VAngleX = 0;
	 VAngleY = 0;


	 cameraJump=0;
	 CarX = 0.0;
	 CarY = 1.0;
	 CarZ = 0.0;

	 CarDX = 0.0;
	 CarDY = 0.0;
	 CarDZ = 0.0;
	 ortho = 12.0;


}
