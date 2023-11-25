#include "Room.h"


Room::~Room()
{
}

Room::Room(int room)
{
	AnimalRoom = room;
	if (AnimalRoom == 0) {
		PositionX = -100;
		PositionY = 14;
		PositionZ = 0;
	}
	if (AnimalRoom == 1) {
		PositionX = 100;
		PositionY = 14;
		PositionZ = 0;
	}
	if (AnimalRoom == 2) {
		PositionX = 0;
		PositionY = 14;
		PositionZ = -100;
	}
}


void Room::Update()
{

	glm::mat4 Scale;
	glm::mat4 Trans;
	if (AnimalRoom == 0) {//고양이
		Scale = glm::scale(Unit, glm::vec3(5, 15, 5));
		Trans = glm::translate(Unit, glm::vec3(PositionX, PositionY, PositionZ));
	}
	if (AnimalRoom == 1) {//강아지
		Scale = glm::scale(Unit, glm::vec3(5, 15, 5));
		Trans = glm::translate(Unit, glm::vec3(PositionX, PositionY, PositionZ));
	}
	if (AnimalRoom == 2) {//곰
		Scale = glm::scale(Unit, glm::vec3(5, 15, 5));
		Trans = glm::translate(Unit, glm::vec3(PositionX, PositionY, PositionZ));
	}                                                        
	Change = Trans * Scale;
}


void Room::Draw()
{
	glBindVertexArray(VAO);
	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 1);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	
	if (herodead) {
		
		for (int i = 0; i < 6; ++i) {
			if (i == 4) {
				glBindTexture(GL_TEXTURE_2D, DeadTexture[AnimalRoom]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
			else{
				glBindTexture(GL_TEXTURE_2D, DeadTexture[AnimalRoom]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
		}
		
	}
	else if(beardead) {
		for (int i = 0; i < 6; ++i) {
			if (i == 4) {
				glBindTexture(GL_TEXTURE_2D, RoomTexture[AnimalRoom]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, ScoreTexture[hero[global_ID].InfoHP() - 2]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
		}
	}
	else {
		for (int i = 0; i < 6; ++i) {
			glBindTexture(GL_TEXTURE_2D, RoomTexture[AnimalRoom]);
			glDrawArrays(GL_TRIANGLES, i * 6, 6);
		}
		/*for (int i = 0; i < 6; ++i) {
			if (i == 4) {
				glBindTexture(GL_TEXTURE_2D, RoomTexture[AnimalRoom]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, ScoreTexture[hero.InfoHP() - 1]);
				glDrawArrays(GL_TRIANGLES, i * 6, 6);
			}
		}*/
	}


}


float Room::getLeft()
{

	return PositionX - 5.f;
}

float Room::getRight()
{
	return PositionX + 5.f;
}

float Room::getBehind()
{
	return PositionZ - 5.f;
}

float Room::getFront()
{
	return PositionZ + 5.f;
}