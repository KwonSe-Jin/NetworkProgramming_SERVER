#include "Leg.h"

Leg::Leg() : i(i)
{

}

Leg::Leg(glm::vec3 Color, int i, int monster) : i(i), rotate(0.f), isrotDirec(false), monster{monster}
{
	this->Color = Color;
	update();
}

Leg::~Leg()
{

}

void Leg::update()
{
	if (monster == 3) {
		if (isrotDirec) {
			rotate += 2.5f;
			if (rotate > 30.f)
				isrotDirec = false;
		}

		else {
			rotate -= 2.5f;
			if (rotate < -30.f)
				isrotDirec = true;
		}

		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.01, 0.03, 0.01));	//다리길이 0.15

		Change = Scale * Trans;		// 위로 올리고 축소

		Trans = glm::translate(Unit, glm::vec3(0, -0.15, 0));
		Change = Trans * Change;	//다리 흔들기 위해 원점에 맞춰주고

		Rotate = glm::rotate(Unit, glm::radians(rotate), glm::vec3(i * -1.f, 0, 0));
		Change = Rotate * Change;	//다리 흔드는 모션 해줌

		Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
		Change = Trans * Change;	//다시 원위치 시켜주고

		Trans = glm::translate(Unit, glm::vec3(i * 0.07, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);		// 내 이동위치로 무브
		Change = Trans * Change;
	}
	else {
		if (isrotDirec) {
			rotate += 2.5f;
			if (rotate > 30.f)
				isrotDirec = false;
		}
		else {
			rotate -= 2.5f;
			if (rotate < -30.f)
				isrotDirec = true;
		}

		Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
		Scale = glm::scale(Unit, glm::vec3(0.04, 0.075, 0.04));	//다리길이 0.15
		Change = Scale * Trans;		// 위로 올리고 축소
		
		Trans = glm::translate(Unit, glm::vec3(0, -0.15, 0));
		Change = Trans * Change;	//다리 흔들기 위해 원점에 맞춰주고

		Rotate = glm::rotate(Unit, glm::radians(rotate), glm::vec3(i * -1.f, 0, 0));
		Change = Rotate * Change;	//다리 흔드는 모션 해줌

		Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
		Change = Trans * Change;	//다시 원위치 시켜주고

		Trans = glm::translate(Unit, glm::vec3(i * 0.07, 0, 0));
		Change = Trans * Change;	// 내가 원하는 위치(왼쪽 오른쪽

		Rotate = glm::rotate(Unit, Direction, glm::vec3(0, 1, 0));

		Change = Rotate * Change;	//캐릭터 돌려주기

		Trans = glm::translate(Unit, Position);		// 내 이동위치로 무브
		Change = Trans * Change;
	}
}

void Leg::BindVAO() 
{
	if (monster == 3) {
		glBindVertexArray(sphereVAO);
	}
	else
		glBindVertexArray(VAO);
}


void Leg::DrawArrays()
{
	
	if (monster == 3) {
		glDrawArrays(GL_TRIANGLES, 0, vertex2.size() * 3);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, vertex1.size() * 3);
}
