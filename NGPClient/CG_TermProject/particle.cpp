#include "particle.h"
extern int i;

Particle::Particle() 
{ 

	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd{ 0, 255 };
	uniform_real_distribution<float> urdX(-0.23f, 0.23f);
	uniform_real_distribution<float> speedurd(0.003f, 0.015f);
	uniform_real_distribution<float> urdZ(-0.23f, 0.23f);
	color = glm::vec3(urd(dre) / 255., urd(dre) / 255., urd(dre) / 255.);
	dirX = urdX(dre);
	dirY = -0.2;
	dirZ = urdZ(dre);
	speed = speedurd(dre);
}

Particle::~Particle() {

}

void Particle::update() 
{	
	dirY -= speed;
	if (dirY <= -0.7f) {
		isParticle = false;
	}

	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.012, 0.012,  0.012));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(CatEndPosX + dirX, dirY, CatEndPosZ + dirZ));
	//glm::mat4 AddTrans = glm::translate(Unit, glm::vec3(0., 1, 0.));
	Change = Trans * Scale;
}
void Particle::draw()
{
	glBindVertexArray(VAO);
	GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	GLuint aColor = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(aColor, color.r, color.g, color.b,1);
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));
	glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);

}


