#include "Crown.h"



Crown::~Crown()
{
}

Crown::Crown()
{

	Ypos = 7;
}

void Crown::Update()
{
	if (Ypos >= 0.f && beardead) {
		Ypos -= 0.1;
	}
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians(90.f), glm::vec3(1, 0, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.1,0.1, 0.1));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, Ypos, -100));

	Change = Trans* Rotate * Scale;
}


void Crown::Draw()
{
	if (beardead) {

		glBindVertexArray(crownVAO);
		GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
		glUniform1i(SelectColor, 2);
		GLuint aColor = glGetUniformLocation(shaderID, "objectColor");
		glUniform4f(aColor, 255 / 255, 255 / 255, 51 / 255, 1.0);
		GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));
		glDrawArrays(GL_TRIANGLES, 0, vertex6.size() * 3);
	}
}

void Crown::initCrown()
{
	Ypos = 7;
}


