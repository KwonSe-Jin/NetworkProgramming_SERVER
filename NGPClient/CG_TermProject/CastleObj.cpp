#include "CastleObj.h"





CastleObj::~CastleObj()
{

}

CastleObj::CastleObj()
{

}


void CastleObj::Update()
{

	glm::mat4 Scale = glm::scale(Unit, glm::vec3(2, 2, 2));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, -1.1, -18));
	Change = Trans * Scale;
}


void CastleObj::Draw()
{

	glBindVertexArray(castleVAO);
	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 2);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));


	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));
	glDrawArrays(GL_TRIANGLES, 0, vertex5.size() * 3);

}


