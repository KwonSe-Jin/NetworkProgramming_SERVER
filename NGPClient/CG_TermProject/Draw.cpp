#include "Draw.h"
#include "DogAttack.h"
#include "BearAttack.h"


//전역 변수
GLuint shaderID;

glm::vec3 TermGunDir;
glm::vec3 cameraPos;

bool RestartPress = false;
Sound playSound;

void draw();
void AnimalCollideCat();
void AnimalCollideDog();
void BulletCollideCat();
void BulletCollideDog();

void CatAndRoomCollision();
void DogAndRoomCollision();
void Bearroomtest();

bool isCollideCatroom(Cat r1, Room r2);
bool isCollideDogroom(Dog r1, Room r2);
bool isCollideBearroom(Bear r1, Room r2);
bool HeroVSRoom(Hero r1, Room r2);
void HeroVSDog();
void HeroVSBear();
void HeroVSCat();

bool isCollideDog(Dog r1, Gun r2);
bool isCollide2D(Cat r1, Gun r2);
bool isCollideBear(Bear r1, Gun r2);


int catdead{};
int dogdead{};
int beardead{};

int HerogetHP = 10;

float lightPosX = 0.0;
float lightPosY = 30.0;
float lightPosZ = 0.0;

int lightcolorN = 0;
float lightRot;

bool isParticle = false;
bool isBullet = false;
bool BearLife = true;

float openingCat = 0;
float openingDog = 0;
int i = 0;

bool catopen = true;
bool dogopen = true;

/// <summary>
/// 주인공
/// </summary>
float HeroMovY = 0;
float MovX = 0;
float MovZ = 0;
bool catlive = false;
bool doglive = false;
bool bearlive = false;
bool herodead = false;
float HeroLocationX = 0;
float HeroLocationZ = 0;
CatAttack catattack[AnimCnt];
DogAttack dogattack[AnimCnt];
BearAttack bearattack;
bool jumpUp = true;

void Restartinit();
vector<Gun*> gun;

int CatCnt = 0;
int DogCnt = 0;



vector<Particle*> particle{ new Particle,new Particle, new Particle, new Particle, new Particle
						,new Particle, new Particle,new Particle,new Particle,new Particle,
						 new Particle,new Particle, new Particle, new Particle, new Particle
						,new Particle, new Particle,new Particle,new Particle,new Particle,new Particle,new Particle, new Particle, new Particle, new Particle
						,new Particle, new Particle,new Particle,new Particle,new Particle,
						 new Particle,new Particle, new Particle, new Particle, new Particle
						,new Particle, new Particle,new Particle,new Particle,new Particle };

vector<Cat*> cats{ new Cat, new Cat, new Cat, new Cat, new Cat, new Cat };
vector<Dog*> dogs{ new Dog, new Dog, new Dog, new Dog, new Dog, new Dog };
Bear bear;

Hero hero{};

World world{};
Tree tree[400];
Grass grass[600];
Crown crown{};
CastleObj castle{};
Room catRoom{ 0 };
Room dogRoom{ 1 };
Room bearRoom{ 2 };
Tunnel Dogopen{ 2 };
DoorL Dogleft{ 2 };
DoorR Dogright{ 2 };

Tunnel Catopen{ 1 };
DoorL Catleft{ 1 };
DoorR Catright{ 1 };


float CatEndPosX;
float CatEndPosZ;



//NetworkManager networkManager("127.0.0.1", 7777);

//random_device rd;
//default_random_engine dre(rd());
//uniform_real_distribution<float> urd{ 0, 255 };

void makeInfo(SC_PLAYER_PACKET* p)
{
	hero.setInfo(p);
}


void get_vangle(float* x, float* y)
{
	hero.get_vangle(x, y);
}

int t_count;

GLvoid drawScene()
{

	GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	if (hero.lightColorR < 0.35)
		glClearColor(0.f, 0.f, 0.f, 1.0f);
	else
		glClearColor(1.f, 1.f, 1.f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DITHER);
	//glEnable(GL_CULL_FACE);  
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	//glm::mat4 projection;

	hero.cameraProjection();
	hero.camera();
	draw();
	hero.DrawHP();
	hero.cameraProjection2();
	//hero.TopView();
	//draw();
	glutSwapBuffers();
};


void draw() {
	///재시작 !! 
	if (RestartPress) {
		Restartinit();
		RestartPress = false;
	}

	if (beardead) {
		hero.lightColorR = 1.0f;
		hero.lightColorG = 1.0f;
		hero.lightColorB = 1.0f;
	}
	if (herodead) {
		hero.lightColorR = 0.5f;
		hero.lightColorG = 0.5f;
		hero.lightColorB = 0.5f;

	}

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");      //--- lightPos 
	glm::vec4 tempv(lightPosX, lightPosY, lightPosZ, 1);
	glm::mat4 Lightrotate = glm::rotate(glm::mat4(1.f), glm::radians(lightRot), glm::vec3(0, 1, 0));
	tempv = Lightrotate * tempv;
	glUniform3f(lightPosLocation, tempv.x, tempv.y, tempv.z);
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");   //--- lightColor 
	glUniform3f(lightColorLocation, hero.lightColorR, hero.lightColorG, hero.lightColorB);
	unsigned int aColor = glGetUniformLocation(shaderID, "objectColor");   //--- object Color
	glUniform4f(aColor, 1, 1., 1., 1.);

	world.Draw();




	glEnable(GL_BLEND); //투명 객체 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	for (int i = 0; i < 400; ++i) {
		tree[i].Draw();
		tree[i].Update();

	}




	for (int i = 0; i < 600; ++i) {
		grass[i].Draw();
		grass[i].Update();
	}

	glDisable(GL_BLEND);


	if (catdead == 6) {
		catopen = true;
		dogopen = false;
	}


	if (dogdead == 6) {
		catopen = false;
		dogopen = true;
	}


	catRoom.Draw();
	catRoom.Update();
	dogRoom.Draw();
	dogRoom.Update();
	bearRoom.Draw();
	bearRoom.Update();

	castle.Draw();
	castle.Update();

	Catleft.Draw();
	Catleft.Update();
	Catright.Draw();
	Catright.Update();

	Dogleft.Draw();
	Dogleft.Update();
	Dogright.Draw();
	Dogright.Update();

	//AnimalCollideCat();
	//Catroomtest();

	//AnimalCollideDog();
	//DogAndRoomCollision();

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
	glUniform1i(selectColorLocation, 0);

	Dogopen.Draw();
	Dogopen.Update();
	Catopen.Draw();
	Catopen.Update();

	crown.Draw();
	crown.Update();




	AnimalCollideCat();
	CatAndRoomCollision();

	AnimalCollideDog();
	DogAndRoomCollision();



	for (int i = 0; i < cats.size(); ++i) {
		cats[i]->draw();
	}

	for (int i = 0; i < dogs.size(); ++i) {
		dogs[i]->draw();
	}

	if (BearLife) {
		bear.draw();
	}
	hero.Update();
	hero.Draw();



	if (catlive)
		HeroVSCat();

	if (doglive)
		HeroVSDog();

	if (bearlive)
		HeroVSBear();

	for (Gun*& gunbullet : gun) {
		gunbullet->Update();
		gunbullet->Draw();

	}
	if (isParticle) {
		for (int i = 0; i < 40; ++i) {
			particle[i]->update();
			particle[i]->draw();
		}
	}
	if (isBullet && BulletLimit == 0) {
		BulletLimit += 1;
		gun.push_back(new Gun{ cameraPos.x,cameraPos.y,cameraPos.z, TermGunDir.x,TermGunDir.y,TermGunDir.z });
	}

	auto p = find_if(gun.begin(), gun.end(), [](Gun* guns) {
		if ((guns->GetPosX()) > 500 || (guns->GetPosX()) < -500 || (guns->GetPosZ()) > 500 || (guns->GetPosZ()) < -500) {
			return true;
		}
		else {
			return false;
		}
		});
	if (p != gun.end() && gun.begin() != gun.end()) {
		gun.erase(p);
	}
	glBindVertexArray(HeroHPVAO);
	aColor = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(aColor, 1.0, 0.0, 0., 1);

}

void AnimalCollideCat() {

	for (int i = 0; i < cats.size(); ++i) {
		for (int j = i + 1; j < cats.size(); ++j) {
			float distanceX = abs(cats[i]->Position.x - cats[j]->Position.x);

			float distanceZ = abs(cats[i]->Position.z - cats[j]->Position.z);
			if (distanceX <= 0.1f) {
				cats[i]->Position.x += 0.1f;
			}
			if (distanceZ <= 0.1f) {
				cats[i]->Position.z += 0.1f;
			}
		}
	}
}

void AnimalCollideDog() {
	for (int i = 0; i < dogs.size(); ++i) {
		for (int j = i + 1; j < dogs.size(); ++j) {
			float distanceX = abs(dogs[i]->Position.x - dogs[j]->Position.x);
			float distanceZ = abs(dogs[i]->Position.z - dogs[j]->Position.z);
			if (distanceX <= 0.1f) {
				dogs[i]->Position.x += 0.1f;
			}
			if (distanceZ <= 0.1f) {
				dogs[i]->Position.z += 0.1f;
			}
		}
	}

}



void BulletCollideCat() {

	for (int i = 0; i < gun.size(); ++i) {
		for (int j = 0; j < cats.size(); ++j) {
			if (isCollide2D(*cats[j], *gun[i])) {
				cats[j]->HP -= gun[i]->Damage;
				delete gun[i];
				if (0 == cats[j]->HP) {
					for (int i = 0; i < 40; ++i) {
						particle[i]->dirY = -0.2;
					}
					CatEndPosX = cats[j]->Position.x;
					CatEndPosZ = cats[j]->Position.z;
					isParticle = true;
					delete cats[j];
					catdead++;
					cats.erase(cats.begin() + j);
					--j;
				}
				gun.erase(gun.begin() + i);
				--i;
				break;
			}

		}

	}
}

void BulletCollideDog() {

	for (int i = 0; i < gun.size(); ++i) {
		for (int j = 0; j < dogs.size(); ++j) {
			if (isCollideDog(*dogs[j], *gun[i])) {
				dogs[j]->HP -= gun[i]->Damage;
				delete gun[i];
				if (0 == dogs[j]->HP) {
					for (int i = 0; i < 40; ++i) {
						particle[i]->dirY = -0.2;
					}
					isParticle = true;
					delete dogs[j];
					dogdead++;
					dogs.erase(dogs.begin() + j);
					--j;
				}
				gun.erase(gun.begin() + i);
				--i;
				break;
			}

		}

	}
}

void BulletCollideBear() {

	for (int i = 0; i < gun.size(); ++i) {
		if (isCollideBear(bear, *gun[i])) {
			bear.HP -= gun[i]->Damage;
			delete gun[i];
			if (0 == bear.HP) {
				for (int i = 0; i < 40; ++i) {
					particle[i]->dirY = -0.2;
				}
				BearLife = false;
				beardead = true;
				isParticle = true;
			}
			gun.erase(gun.begin() + i);
			--i;
		}
	}
}

bool isCollide2D(Cat r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

bool isCollideDog(Dog r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}


bool isCollideBear(Bear r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}


bool isCollideCatroom(Cat r1, Room r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

bool isCollideDogroom(Dog r1, Room r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

bool isCollideBearroom(Bear r1, Room r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

bool HeroVSRoom(Hero r1, Room r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

void CatAndRoomCollision()
{
	for (int i = 0; i < cats.size(); ++i) {
		if (isCollideCatroom(*cats[i], catRoom)) {

			if (cats[i]->Position.x < catRoom.PositionX - 5)
				cats[i]->Position.x += 1;
			if (cats[i]->Position.x > catRoom.PositionX + 5)
				cats[i]->Position.x -= 1;
			if (cats[i]->Position.z < catRoom.PositionZ - 5)
				cats[i]->Position.z += 1;
			if (cats[i]->Position.z > catRoom.PositionZ + 5)
				cats[i]->Position.z -= 1;
		}
	}
}


void DogAndRoomCollision()
{
	for (int i = 0; i < dogs.size(); ++i) {
		if (isCollideDogroom(*dogs[i], dogRoom)) {
			if (dogs[i]->Position.x < dogRoom.PositionX - 5)
				dogs[i]->Position.x += 1;
			if (dogs[i]->Position.x > dogRoom.PositionX + 5)
				dogs[i]->Position.x -= 1;
			if (dogs[i]->Position.z < dogRoom.PositionZ - 5)
				dogs[i]->Position.z += 1;
			if (dogs[i]->Position.z > dogRoom.PositionZ + 5)
				dogs[i]->Position.z -= 1;
		}
	}
}


void Bearroomtest()
{

	if (isCollideBearroom(bear, bearRoom)) {

		if (bear.Position.x < bearRoom.PositionX - 5)
			bear.Position.x += 1;
		if (bear.Position.x > bearRoom.PositionX + 5)
			bear.Position.x -= 1;
		if (bear.Position.z < bearRoom.PositionZ - 5)
			bear.Position.z += 1;
		if (bear.Position.z > bearRoom.PositionZ + 5)
			bear.Position.z -= 1;
	}

}



void HeroVSBear()
{
	if (HeroVSRoom(hero, bearRoom)) {


		if (hero.PosX < bearRoom.PositionX - 5)
			hero.PosX += 1.0;
		if (hero.PosX > bearRoom.PositionX + 5)
			hero.PosX -= 1.0;
		if (hero.PosZ < bearRoom.PositionZ - 5)
			hero.PosZ += 1.0;
		if (hero.PosZ > bearRoom.PositionZ + 5)
			hero.PosZ -= 1.0;
	}
}

void HeroVSCat()
{
	if (HeroVSRoom(hero, catRoom)) {

		if (catdead == 6 && hero.PosX > -101 && hero.PosX < -98 && hero.PosZ < -4.5 && isW) {
			hero.PosZ -= 0.1;
		}
		else {
			if (hero.PosX < catRoom.PositionX - 5)
				hero.PosX += 1.0;
			if (hero.PosX > catRoom.PositionX + 5)
				hero.PosX -= 1.0;
			if (hero.PosZ < catRoom.PositionZ - 5)
				hero.PosZ += 1.0;
			if (hero.PosZ > catRoom.PositionZ + 5)
				hero.PosZ -= 1.0;
		}

	}
}

void HeroVSDog()
{
	if (HeroVSRoom(hero, dogRoom)) {

		if (dogdead == 6 && hero.PosX < +101 && hero.PosX > 98 && hero.PosZ < -4.5 && isW) {
			hero.PosZ -= 0.1;
		}
		else {
			if (hero.PosX < dogRoom.PositionX - 5)
				hero.PosX += 1.0;
			if (hero.PosX > dogRoom.PositionX + 5)
				hero.PosX -= 1.0;
			if (hero.PosZ < dogRoom.PositionZ - 5)
				hero.PosZ += 1.0;
			if (hero.PosZ > dogRoom.PositionZ + 5)
				hero.PosZ -= 1.0;
		}

	}
}

void Restartinit()
{
	playSound.stopBGM();
	TermGunDir = { 0,0,0 };
	cameraPos = { 0,0,0 };
	catdead = 0;
	dogdead = 0;
	beardead = 0;
	HerogetHP = 10;
	lightPosX = 0.0;
	lightPosY = 30.0;
	lightPosZ = 0.0;
	lightcolorN = 0;
	lightRot = 0;

	isParticle = false;
	isBullet = false;
	openingCat = 0;
	openingDog = 0;
	i = 0;
	DogCnt = 0;
	CatCnt = 0;

	// cat 초기화 
	for (int i = 0; i < cats.size(); ++i) {
		cats[i]->initCat();
		cats[i]->Index = CatCnt;
		++CatCnt;
	}

	if (cats.size() != 6)
	{
		while (cats.size() < 6)
		{
			cats.emplace_back(new Cat());
		}

	}
	// 강아지 초기화 

	for (int i = 0; i < dogs.size(); ++i) {
		dogs[i]->initDog();
		dogs[i]->Index = DogCnt;
		++DogCnt;
	}

	if (dogs.size() != 6)
	{
		while (dogs.size() < 6)
		{
			dogs.emplace_back(new Dog());
		}
	}

	//곰 초기화
	bear.InitBear();
	BearLife = true;

	//주인공 초기화 

	catlive = false;
	doglive = false;
	bearlive = false;

	herodead = false;

	catopen = true;
	dogopen = true;

	HeroLocationX = 0;
	HeroLocationZ = 0;
	DogCnt = 0;
	CatCnt = 0;

	for (int i = 0; i < AnimCnt; ++i)
	{
		catattack[i].initCatAttack();
	}
	for (int i = 0; i < AnimCnt; ++i)
	{
		dogattack[i].initDogAttack();
	}
	bearattack.initBearAttack();
	bool jumpUp = true;

	hero.initHero();


	///왕관 초기화
	crown.initCrown();



}