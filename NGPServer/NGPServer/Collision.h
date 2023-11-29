#pragma once
#include "pch.h"
#include "gun.h"

extern vector<Animal*> AniCats;
extern vector<Animal*> AniDogs;
extern Animal AniBear;

extern vector<Gun*> gun;
extern vector<Hero> heroes; //주인공 벡터 일단 만들어놓음 나중에 맵으로 수정 후 주석 지워주세요. 
extern Room catRoom;
extern Room dogRoom;
extern Room bearRoom;

extern bool isCollideDog(Animal r1, Gun r2);
extern bool isCollide2D(Animal r1, Gun r2);
extern bool isCollideBear(Animal r1, Gun r2);

void AnimalCollideCat() {

	for (int i = 0; i < AniCats.size(); ++i) {
		for (int j = i + 1; j < AniCats.size(); ++j) {
			float distanceX = abs(AniCats[i]->PosX - AniCats[j]->PosX);

			float distanceZ = abs(AniCats[i]->PosZ - AniCats[j]->PosZ);
			if (distanceX <= 0.1f) {
				AniCats[i]->PosX += 0.1f;
			}
			if (distanceZ <= 0.1f) {
				AniCats[i]->PosZ += 0.1f;
			}
		}
	}
}

void AnimalCollideDog() {
	for (int i = 0; i < AniDogs.size(); ++i) {
		for (int j = i + 1; j < AniDogs.size(); ++j) {
			float distanceX = abs(AniDogs[i]->PosX - AniDogs[j]->PosX);
			float distanceZ = abs(AniDogs[i]->PosZ - AniDogs[j]->PosZ);
			if (distanceX <= 0.1f) {
				AniDogs[i]->PosX += 0.1f;
			}
			if (distanceZ <= 0.1f) {
				AniDogs[i]->PosZ += 0.1f;
			}
		}
	}

}



void BulletCollideCat() {

	for (int i = 0; i < gun.size(); ++i) {
		for (int j = 0; j < AniCats.size(); ++j) {
			if (isCollide2D(*AniCats[j], *gun[i])) {
				AniCats[j]->HP -= gun[i]->Damage;
				//delete gun[i];
				gun[i]->status = false;
				if (0 == AniCats[j]->HP) {
					/*for (int i = 0; i < 40; ++i) {
						particle[i]->dirY = -0.2;
					}
					CatEndPosX = cats[j]->Position.x;
					CatEndPosZ = cats[j]->Position.z;
					isParticle = true;
					delete cats[j];
					catdead++;
					cats.erase(cats.begin() + j);
					--j;*/
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
		for (int j = 0; j < AniDogs.size(); ++j) {
			if (isCollideDog(*AniDogs[j], *gun[i])) {
				AniDogs[j]->HP -= gun[i]->Damage;
				delete gun[i];
				if (0 == AniDogs[j]->HP) {
					/*for (int i = 0; i < 40; ++i) {
						particle[i]->dirY = -0.2;
					}
					isParticle = true;
					delete dogs[j];
					dogdead++;
					dogs.erase(dogs.begin() + j);
					--j;*/
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
		if (isCollideBear(AniBear, *gun[i])) {
			AniBear.HP -= gun[i]->Damage;
			delete gun[i];
			if (0 == AniBear.HP) {
				/*for (int i = 0; i < 40; ++i) {
					particle[i]->dirY = -0.2;
				}
				BearLife = false;
				beardead = true;
				isParticle = true;*/
			}
			gun.erase(gun.begin() + i);
			--i;
		}
	}
}

bool isCollide2D(Animal r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}

bool isCollideDog(Animal r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}


bool isCollideBear(Animal r1, Gun r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}


bool isCollideAnimals_room(Animal r1, Room r2)
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


void Catroomtest()
{
	for (int i = 0; i < AniCats.size(); ++i) {
		if (isCollideAnimals_room(*AniCats[i], catRoom)) {

			if (AniCats[i]->PosX < catRoom.PositionX - 5)
				AniCats[i]->PosX += 1;
			if (AniCats[i]->PosX > catRoom.PositionX + 5)
				AniCats[i]->PosX -= 1;
			if (AniCats[i]->PosZ < catRoom.PositionZ - 5)
				AniCats[i]->PosZ += 1;
			if (AniCats[i]->PosZ > catRoom.PositionZ + 5)
				AniCats[i]->PosZ -= 1;
		}
	}
}

void DogAndRoomCollision()
{
	for (int i = 0; i < AniDogs.size(); ++i) {
		if (isCollideAnimals_room(*AniDogs[i], dogRoom)) {
			if (AniDogs[i]->PosX < dogRoom.PositionX - 5)
				AniDogs[i]->PosX += 1;
			if (AniDogs[i]->PosX > dogRoom.PositionX + 5)
				AniDogs[i]->PosX -= 1;
			if (AniDogs[i]->PosZ < dogRoom.PositionZ - 5)
				AniDogs[i]->PosZ += 1;
			if (AniDogs[i]->PosZ > dogRoom.PositionZ + 5)
				AniDogs[i]->PosZ -= 1;
		}
	}
}



void BearAndRoomCollision()
{

	if (isCollideAnimals_room(AniBear, bearRoom)) {

		if (AniBear.PosX < bearRoom.PositionX - 5)
			AniBear.PosX += 1;
		if (AniBear.PosX > bearRoom.PositionX + 5)
			AniBear.PosX -= 1;
		if (AniBear.PosZ < bearRoom.PositionZ - 5)
			AniBear.PosZ += 1;
		if (AniBear.PosZ > bearRoom.PositionZ + 5)
			AniBear.PosZ -= 1;
	}

}


void HeroVSBear()
{
	for (int i = 0; i < heroes.size(); ++i) {
		if (HeroVSRoom(heroes[i], bearRoom)) {
			if (heroes[i].PosX < bearRoom.PositionX - 5)
				heroes[i].PosX += 1.0;
			if (heroes[i].PosX > bearRoom.PositionX + 5)
				heroes[i].PosX -= 1.0;
			if (heroes[i].PosZ < bearRoom.PositionZ - 5)
				heroes[i].PosZ += 1.0;
			if (heroes[i].PosZ > bearRoom.PositionZ + 5)
				heroes[i].PosZ -= 1.0;
		}
	}
}

void HeroVSCat()
{
	for (int i = 0; i < heroes.size(); ++i) {
		if (HeroVSRoom(heroes[i], catRoom)) {
			if (heroes[i].PosX < catRoom.PositionX - 5)
				heroes[i].PosX += 1.0;
			if (heroes[i].PosX > catRoom.PositionX + 5)
				heroes[i].PosX -= 1.0;
			if (heroes[i].PosZ < catRoom.PositionZ - 5)
				heroes[i].PosZ += 1.0;
			if (heroes[i].PosZ > catRoom.PositionZ + 5)
				heroes[i].PosZ -= 1.0;
		}
	}
}

void HeroVSDog()
{
	for (int i = 0; i < heroes.size(); ++i) {

		if (HeroVSRoom(heroes[i], dogRoom)) {

			if (heroes[i].PosX < dogRoom.PositionX - 5)
				heroes[i].PosX += 1.0;
			if (heroes[i].PosX > dogRoom.PositionX + 5)
				heroes[i].PosX -= 1.0;
			if (heroes[i].PosZ < dogRoom.PositionZ - 5)
				heroes[i].PosZ += 1.0;
			if (heroes[i].PosZ > dogRoom.PositionZ + 5)
				heroes[i].PosZ -= 1.0;
		}
	}
}

