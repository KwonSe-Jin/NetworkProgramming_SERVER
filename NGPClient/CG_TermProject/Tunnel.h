#pragma once
#include "Header.h"


class Tunnel
{
	glm::mat4 Change;
	int Open;

public:

	Tunnel(int open);

	~Tunnel();
	void Update();
	void Draw();

};


