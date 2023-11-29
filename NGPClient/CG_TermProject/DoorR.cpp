#include "DoorR.h"


DoorR::~DoorR()
{
}

DoorR::DoorR(int open)
{
	Open = open;
}


void DoorR::Update()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.5, 1, 0.125));
	glm::mat4 Trans;
	if (Open == 1) {
		Trans = glm::translate(Unit, glm::vec3(-99.5+openingCat, 0, -4.9));

	}
	else
		Trans = glm::translate(Unit, glm::vec3(100.5+openingDog, 0, -4.9));
	Change = Trans * Scale;
}

void DoorR::Draw()
{
	if (dogopen || catopen) {
		glBindVertexArray(VAO);
		GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
		glUniform1i(selectColorLocation, 1);
		GLuint model = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));
		glBindTexture(GL_TEXTURE_2D, DoorTexture[1]);
		glDrawArrays(GL_TRIANGLES, 30, 6);

	}
}


