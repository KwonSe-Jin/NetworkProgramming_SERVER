
#pragma once
#include "Header.h"

//좌표 XYZ / 스케일 / 색상 

class Crown
{
	glm::mat4 Change;
	float Xpos;
	float Ypos;
	float Zpos;

public:

	Crown();

	~Crown();
	void Update();
	void Draw();

	void initCrown();

};


