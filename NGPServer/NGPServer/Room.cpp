#include "Room.h"
#include "pch.h"

Room::~Room()
{
}

Room::Room(int room)
{
	AnimalRoom = room;
	if (AnimalRoom == Cat) {
		PositionX = -100;
		PositionY = 14;
		PositionZ = 0;
	}
	if (AnimalRoom == Dog) {
		PositionX = 100;
		PositionY = 14;
		PositionZ = 0;
	}
	if (AnimalRoom == Bear) {
		PositionX = 0;
		PositionY = 14;
		PositionZ = -100;
	}
}

float Room::getLeft()
{

	return PositionX - 5.f;
}

float Room::getRight()
{
	return PositionX + 5.f;
}

float Room::getBehind()
{
	return PositionZ - 5.f;
}

float Room::getFront()
{
	return PositionZ + 5.f;
}