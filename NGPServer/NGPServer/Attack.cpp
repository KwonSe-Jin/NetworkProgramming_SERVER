#include "Attack.h"
//이 클래스는 히어로의 공격량을 계산하기 위해 있는 클래스입니다.
void Attack::initAttack()
{
	Activate = false;
	AttackCount = 0;
}

Attack::Attack() {
	Activate = false;
	AttackCount = 0;
}