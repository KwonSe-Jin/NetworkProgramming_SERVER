#include "DoorL.h"
extern float openingCat;
extern float openingDog;
DoorL::~DoorL()
{
}

DoorL::DoorL(int open)
{
	Open = open;
}


void DoorL::Update()
{
	//FixUpdate();
	//Tx = glm::translate(Tx, glm::vec3(0, 0, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.5, 1, 0.125));
	glm::mat4 Trans;
	if (Open == 1) {
		Trans = glm::translate(Unit, glm::vec3(-100.5-openingCat, 0, -4.9));

	}
	else
		Trans = glm::translate(Unit, glm::vec3(99.5-openingDog, 0, -4.9));
	//glm::mat4 Trans1 = glm::translate(Unit, glm::vec3(0, 1, 0));
	Change = Trans * Scale;
}

void DoorL::Draw()
{
	if (dogopen || catopen) {

		glBindVertexArray(VAO);
		GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
		glUniform1i(selectColorLocation, 1);
		GLuint model = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));
		glBindTexture(GL_TEXTURE_2D, DoorTexture[0]);
		glDrawArrays(GL_TRIANGLES, 30, 6);

	}
}


//for (int i = 0; i < 6; ++i) {
//	glBindTexture(GL_TEXTURE_2D, Texture[i]);
//	glDrawArrays(GL_TRIANGLES, i * 6, 6);
//}