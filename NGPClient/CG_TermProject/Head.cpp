#include "Head.h"

Head::Head()
{
}

Head::Head(glm::vec3 Color, int monster) : monster{ monster }
{
	this->Color = Color;
	update();
}

Head::~Head()
{
}

void Head::update()
{
	if (monster == 3) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.025, 0.03, 0.025));	//머리사이즈 0.1

		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.5, 0));
		Change = Trans * Change;	//다리 위로 올려줌

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.08, 0.08, 0.08));	//머리사이즈 0.1

		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.45, 0));
		Change = Trans * Change;	//다리 위로 올려줌

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	
}


void Head::BindVAO()
{
	if (monster == 3) {
		glBindVertexArray(sphereVAO);
	}
	else 
		glBindVertexArray(VAO);
}


void Head::DrawArrays()
{
	
	if (monster == 3) {
		glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);
}