#pragma once
#include "pch.h"

//extern CatAttack catattack[AnimCnt];
//extern DogAttack dogattack[AnimCnt];
//extern BearAttack bearattack;

class Hero
{
	
	bool firstmap;
	/// <summary>
	/// 카메라 시작합니다.
	/// </summary>
	float VAngleX = 1;
	float VAngleY = 1;



public:
	int ID;
	int HP;
	float PosX;
	float PosY;
	float PosZ;
	float lightColorR = 1.0f;
	float lightColorG = 1.0f;
	float lightColorB = 1.0f;
	Hero(int id);
	~Hero();
	void ISW();
	void ISA();
	void ISS();
	void ISD();
	void damage();
	void Update();
	int InfoHP();
	void location();
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	void VAngleMotion(int x, int y);
	void initHero();

	/*friend void HeroVSDog();
	friend void HeroVSBear();
	friend void HeroVSCat();*/
	///////////////카메라 시작합니다.///////////////

};