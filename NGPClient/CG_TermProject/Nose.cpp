#include "Nose.h"

Nose::Nose()
{
}

Nose::Nose(glm::vec3 Color, int monster) :monster{ monster }
{
	this->Color = Color;
	update();
}

Nose::~Nose()
{
}

void Nose::update()
{
	if (monster == 3) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.003, 0.003, 0.003));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.55, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.12));
		Change = Trans * Change;	//원하는 위치로 이동

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.003, 0.003, 0.003));	//위에서 봤기때문에 납작해보여서 y 늘려줌
		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.53, 0));
		Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.075));
		Change = Trans * Change;	//원하는 위치로 이동

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
}


void Nose::BindVAO()
{
	glBindVertexArray(sphereVAO);
}

void Nose::DrawArrays()
{
	glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);

}
