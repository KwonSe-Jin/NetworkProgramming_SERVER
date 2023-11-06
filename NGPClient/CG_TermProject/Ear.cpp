#include "Ear.h"

Ear::Ear()
{
}

Ear::Ear(glm::vec3 Color, int i, int monster) :i{ i }, monster{ monster }
{
	this->Color = Color;
	update();
}

Ear::~Ear()
{
}

void Ear::update()
{
	if (monster == 1) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.037, 0.037, 0.037));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.64, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.0));
		Change = Trans * Change;	//원하는 위치로 이동

		Trans = glm::translate(Unit, glm::vec3(i * 0.043, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽)

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else if(monster ==2){
		//Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.039, 0.039, 0.039));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale;

		Trans = glm::translate(Unit, glm::vec3(0, 0.57, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.0));
		Change = Trans * Change;	//원하는 위치로 이동

		Trans = glm::translate(Unit, glm::vec3(i * 0.073, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽)

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else if (monster == 3) {
		//Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.01, 0.01, 0.01));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale;

		Trans = glm::translate(Unit, glm::vec3(0, 0.65, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.0));
		Change = Trans * Change;	//원하는 위치로 이동

		Trans = glm::translate(Unit, glm::vec3(i * 0.078, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽)

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
}


void Ear::BindVAO()
{
	if(monster==1)
		glBindVertexArray(pyramidVAO);
	else if(monster ==2)
		glBindVertexArray(VAO);
	else if (monster == 3)
		glBindVertexArray(sphereVAO);
}

void Ear::DrawArrays()
{
	if (monster == 1)
		glDrawArrays(GL_TRIANGLES, 0, vertex3.size() * 3);
	else if (monster == 2)
		glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);
	else if (monster == 3)
		glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);

}