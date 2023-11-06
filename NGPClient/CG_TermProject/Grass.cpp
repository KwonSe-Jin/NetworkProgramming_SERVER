#include "Grass.h"


Grass::~Grass()
{

}

Grass::Grass()
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> X{ -40.f,40.f };
	uniform_real_distribution<float> Z{ -40.f,40.0f };
	uniform_int_distribution<int> Grass{ 0,2 };
	Xpos = X(dre);
	Zpos = Z(dre);
	Grassnum = Grass(dre);
}


void Grass::Update()
{
	float dz = HeroLocationZ - Xpos;
	float dx = HeroLocationX - Zpos;
	Direction = atan2(dx, dz);

	glm::mat4 Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 UPT = glm::translate(Unit, glm::vec3(0, 1, 0));
	glm::mat4 DownT = glm::translate(Unit, glm::vec3(0, -1, 0));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(Xpos, 0, Zpos));

	Change = Trans *DownT* Rotate * Scale*UPT;
}


void Grass::Draw()
{
	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);
	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 1);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));


	glBindTexture(GL_TEXTURE_2D, GrassTexture[Grassnum]);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glEnable(GL_DEPTH_TEST);
	glUniform1i(selectColorLocation, 0);

}


