#pragma once
#include "Header.h"
#include "CatAttack.h"
#include "DogAttack.h"
#include "BearAttack.h"


extern CatAttack catattack[AnimCnt];
extern DogAttack dogattack[AnimCnt];
extern BearAttack bearattack;

class Hero
{
	const glm::mat4 Unit;

	glm::mat4 Change;

	glm::vec3 color;
	float scaleX;
	float scaleY;
	float scaleZ;
	float PosX;
	float PosY;
	float PosZ;
	bool firstmap;
/// <summary>
/// ī�޶� �����մϴ�.
/// </summary>
	bool first = true;
	bool second = true;
	bool third = true;

	float carAddX=0;
	float carAddY=0;
	float carAddZ{ 10 };
	float VAngleX=0;
	float VAngleY=0;


	float cameraJump{};

	glm::vec3 TermGunDir{0,0,0};
	glm::vec3 cameraPos{0,0,0};


	float CarX = 0.0;
	float CarY = 1.0;
	float CarZ = 0.0;

	float CarDX = 0.0;
	float CarDY = 0.0;
	float CarDZ = 0.0;
	float ortho = 12.0;

	glm::mat4 projection;



	unsigned int projectionLocation;


public:
	bool status;
	int HP;

	int Hero_ID = -1;
	float lightColorR = 1.0f;
	float lightColorG = 1.0f;
	float lightColorB = 1.0f;
	Hero(int colorID);
	~Hero();
	/*void ISW();
	void ISA();
	void ISS();
	void ISD();*/
	//void Jump();
	void damage();
	void Update();
	void Draw();
	int InfoHP();
	void location();
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	void get_Info(float* x, float* y, bool* status, float* dirx, float* diry, float* dirz);

	
	friend void HeroVSDog();
	friend void HeroVSBear();
	friend void HeroVSCat();
	///////////////ī�޶� �����մϴ�.///////////////
	void camera();
	void TopView();
	void camera3D();

	void cameraProjection();
	void DrawHP();
	void cameraProjection2();
	void VAngleMotion(int x, int y);
	void setInfo(SC_PLAYER_PACKET* p);


	void initHero();
};