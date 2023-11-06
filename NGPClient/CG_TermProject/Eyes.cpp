#include "Eyes.h"

Eyes::Eyes()
{
}

Eyes::Eyes(glm::vec3 Color,int i, int monster):i{i}, monster{monster}
{
	this->Color = Color;
	update();
}

Eyes::~Eyes()
{
}

void Eyes::update()
{
	if (monster == 3) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.0033, 0.0033, 0.0033));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.57, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.1));
		Change = Trans * Change;	//원하는 위치로 이동

		Trans = glm::translate(Unit, glm::vec3(i * 0.043, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽)

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.0033, 0.0033, 0.0033));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.57, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.075));
		Change = Trans * Change;	//원하는 위치로 이동

		Trans = glm::translate(Unit, glm::vec3(i * 0.043, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽)
		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
}


void Eyes::BindVAO()
{
	glBindVertexArray(sphereVAO);
}

void Eyes::DrawArrays()
{
	glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);

}