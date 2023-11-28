#pragma once
#include "pch.h"
#include "Attack.h"

class Hero
{
	bool firstmap;

public:
	bool is_q;
	Attack catattack[6];
	Attack dogattack[6];
	Attack bearattack;

	bool _flag = true;
	bool _readyflag = false;
	bool catlive;
	bool doglive;
	bool bearlive;
	bool herodead;

	int ID;
	int HP;
	float PosX;
	float PosY;
	float PosZ;
	float lightColorR = 1.0f;
	float lightColorG = 1.0f;
	float lightColorB = 1.0f;
	float VAngleX = 1;
	float VAngleY = 1;
	Hero(int id);
	~Hero();
	void ISW();
	void ISA();
	void ISS();
	void ISD();
	void ISR();
	void damage();
	void Update();
	int InfoHP();
	void location();
	void isQuit();
	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	//void VAngleMotion(int x, int y);
	void initHero();
	float getPosX() const { return PosX; }
	float getPosZ() const { return PosZ; }
	/*friend void HeroVSDog();
	friend void HeroVSBear();
	friend void HeroVSCat();*/
	///////////////ī�޶� �����մϴ�.///////////////

};