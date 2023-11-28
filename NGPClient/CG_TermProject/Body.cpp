#include "Body.h"

Body::Body()
{
}

Body::Body(glm::vec3 Color, int monster) : monster{ monster }
{
	this->Color = Color;
	update();
}

Body::~Body()
{
}

void Body::update()
{
	if (monster == 3) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.035, 0.05, 0.035));	//몸통길이 0.3, 너비 0.15
		//Scale = glm::scale(Unit, glm::vec3(0.1, 0.15, 0.1));
		Change = Scale * Trans;		//위로 올리고 축소
		
		Trans = glm::translate(Unit, glm::vec3(0, 0.2, 0));
		Change = Trans * Change;	//다리 위로 올려줌

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else if (monster == 4) {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.02, 0.027, 0.02));	//몸통길이 0.3, 너비 0.15
		//Scale = glm::scale(Unit, glm::vec3(0.1, 0.15, 0.1));
		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.23, 0.1));
		Change = Trans * Change;	//다리 위로 올려줌

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
	else {
		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.13, 0.15, 0.13));	//몸통길이 0.3, 너비 0.15
		//Scale = glm::scale(Unit, glm::vec3(0.1, 0.15, 0.1));
		Change = Scale * Trans;		//위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
		Change = Trans * Change;	//다리 위로 올려줌

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);
		Change = Trans * Change;	// 내 포지션으로 이동
	}
}


void Body::BindVAO()
{
	if (monster == 3 || monster ==4) {
		glBindVertexArray(sphereVAO);
	}
	else
		glBindVertexArray(VAO);
}

void Body::DrawArrays()
{
	
	if (monster == 3||monster ==4) {
		glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);
}
