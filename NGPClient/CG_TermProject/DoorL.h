
#pragma once
#include "Header.h"


class DoorL
{
	glm::mat4 Change;
	int Open;

public:

	DoorL(int open);

	~DoorL();
	void Update();
	void Draw();

};


