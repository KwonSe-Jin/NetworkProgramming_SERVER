
#pragma once
#include "Header.h"

//좌표 XYZ / 스케일 / 색상 

class Tree
{
	glm::mat4 Change;
	float Xpos;
	float Ypos;
	float Zpos;
	int Treenum;
	float Direction;
	int randN;
public:

	Tree();

	~Tree();
	void Update();
	void Draw();

};

