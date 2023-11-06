#pragma once

#include "Header.h"

class Animals
{
protected:
	const glm::mat4 Unit;
	glm::mat4 Scale;
	glm::mat4 Trans;
	glm::mat4 Rotate;
	glm::mat4 Change;
	glm::vec3 Color;
	glm::vec3 Position;

	float Direction;

public:
	Animals();
	virtual ~Animals();
	void draw();
	void keyIn(glm::vec3 Position, float Direction);
	virtual void update() = 0;
	virtual void BindVAO() = 0;
	virtual void DrawArrays() = 0;
};