#pragma once
#include "Head.h"


class Particle {
	float Xpos;
	float Ypos;
	float Zpos;
	float dirX;
	float dirY;
	float dirZ;
	glm::mat4 Change;
	glm::vec3 color;
	int life = 10;

	float speed;

public:
	Particle();
	~Particle();

	void draw();
	void update();
	friend void BulletCollideCat();
	friend void BulletCollideDog();
	friend void BulletCollideBear();
};

