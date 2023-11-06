#include "Tree.h"


Tree::~Tree()
{

}

Tree::Tree()
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> X1{ 8.f,40.f };
	uniform_real_distribution<float> X2{ -40.f,-8.0f };
	uniform_real_distribution<float> Z{ -40.f,40.0f };
	uniform_int_distribution<int> tree{ 0,2 };
	uniform_int_distribution<int> Rand{ 0,1 };
	randN = Rand(dre);

	if (0 == randN) {
		Xpos = X1(dre);
	}
	else {
		Xpos = X2(dre);
	}
	Zpos = Z(dre);
	Treenum = tree(dre);
}


void Tree::Update()
{
	float dz = HeroLocationZ - Xpos;
	float dx = HeroLocationX - Zpos;
	Direction = atan2(dx, dz);

	glm::mat4 Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(2.0, 2,0.1));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(Xpos, 1, Zpos));

	Change = Trans* Rotate* Scale;
}


void Tree::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 1);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));


	glBindTexture(GL_TEXTURE_2D, TreeTexture[Treenum]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);


}


