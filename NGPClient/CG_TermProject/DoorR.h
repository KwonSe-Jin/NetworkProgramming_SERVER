
#pragma once
#include "Header.h"


class DoorR
{
	glm::mat4 Change;
	int Open;

public:

	DoorR(int open);

	~DoorR();
	void Update();
	void Draw();

};


